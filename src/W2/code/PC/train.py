#导入需要的包 
import paddle as paddle 
import paddle.fluid as fluid 
import os 

BATCH_SIZE = 128 # 每次取数据的个数 
#将训练数据和测试数据读入内存 
train_reader = paddle.batch( 
    paddle.reader.shuffle(paddle.dataset.cifar.train10(), #获取cifa10训练数据 
                          buf_size=128 * 100),                  #在buf_size的空间内进行乱序 
    batch_size=BATCH_SIZE)                                #batch_size:每个批次读入的训练数据 
test_reader = paddle.batch( 
    paddle.dataset.cifar.test10(),                        #获取cifa10测试数据      
    batch_size=BATCH_SIZE)                                #batch_size:每个批次读入的测试数据 

temp_reader = paddle.batch(paddle.dataset.cifar.train10(), 
                            batch_size=3) 
temp_data=next(temp_reader()) 
print('temp_data_shape',type(temp_data)) 
print('temp_data_shape：',len(temp_data)) 
print(temp_data) 

#定义输入数据 
data_shape = [3, 32, 32]#3表图像RGB三通道，32*32的彩色图片 
# 定义全局变量image和label 
images = fluid.layers.data(name='images', shape=data_shape, dtype='float32') 
label = fluid.layers.data(name='label', shape=[1], dtype='int64') 
print('images_shape:',images.shape) 

def convolutional_neural_network(img): 
    # 第一个卷积-池化层 
    conv_pool_1 = fluid.nets.simple_img_conv_pool( 
        input=img,         # 输入图像 
        filter_size=5,     # 滤波器的大小 
        num_filters=20,    # filter 的数量。它与输出的通道相同 
        pool_size=2,       # 池化层大小2*2 
        pool_stride=2,     # 池化层步长 
        act="relu")        # 激活类型 
    # 第二个卷积-池化层 
    conv_pool_2 = fluid.nets.simple_img_conv_pool( 
        input=conv_pool_1, 
        filter_size=5, 
        num_filters=50, 
        pool_size=2, 
        pool_stride=2, 
        act="relu") 
    # 第3个卷积-池化层 
    conv_pool_3 = fluid.nets.simple_img_conv_pool( 
        input=conv_pool_2, 
        filter_size=5, 
        num_filters=50, 
        pool_size=2, 
        pool_stride=2, 
        act="relu") 
    # 以softmax为激活函数的全连接输出层，10类数据输出10个数字 
    prediction = fluid.layers.fc(input=conv_pool_3, size=10, act='softmax') 
    return prediction 

#定义输入数据 
data_shape = [3, 32, 32] 
# 定义全局变量image和label 
images = fluid.layers.data(name='images', shape=data_shape, dtype='float32') 
label = fluid.layers.data(name='label', shape=[1], dtype='int64') 
# 获取分类器，用cnn进行分类 
predict =  convolutional_neural_network(images) 

# 获取损失函数和准确率 
cost = fluid.layers.cross_entropy(input=predict, label=label) # 交叉熵 
avg_cost = fluid.layers.mean(cost)                            # 计算cost中所有元素的平均值 
acc = fluid.layers.accuracy(input=predict, label=label)       #使用输入和标签计算准确率 

# 定义优化方法 
optimizer =fluid.optimizer.Adam(learning_rate=0.001)#Adam是一阶基于梯度下降的算法，基于自适应低阶矩估计该函数实现了自适应矩估计优化器 
optimizer.minimize(avg_cost)# 取最小的优化平均损失 
print(type(acc)) 

# 使用CPU进行训练 
place = fluid.CPUPlace() 
# 创建一个executor 
exe = fluid.Executor(place) 
# 对program进行参数初始化1.网络模型2.损失函数3.优化函数 
exe.run(fluid.default_startup_program()) 
# 定义输入数据的维度,DataFeeder 负责将reader(读取器)返回的数据转成一种特殊的数据结构，使它们可以输入到 Executor 
feeder = fluid.DataFeeder( feed_list=[images, label],place=place) 

# 训练的轮数 
EPOCH_NUM = 20 
# 开始训练 
for pass_id in range(EPOCH_NUM): 
    # 开始训练 
    train_cost = 0 
    for batch_id, data in enumerate(train_reader()):              #遍历train_reader的迭代器，并为数据加上索引batch_id 
        train_cost,train_acc = exe.run(program=fluid.default_main_program(),#运行主程序 
                             feed=feeder.feed(data),              #喂入一个batch的数据 
                             fetch_list=[avg_cost, acc])          #fetch均方误差和准确率 
        if batch_id % 100 == 0:                                   #每100次batch打印一次训练、进行一次测试 
            print('Pass:%d, Batch:%d, Cost:%0.5f, Accuracy:%0.5f' % (pass_id, batch_id, train_cost[0], train_acc[0])) 
    # 开始测试 
    test_costs = []                                                        #测试的损失值 
    test_accs = []                                                         #测试的准确率 
    for batch_id, data in enumerate(test_reader()): 
        test_cost, test_acc = exe.run(program=fluid.default_main_program(),#运行测试程序 
                                      feed=feeder.feed(data),              #喂入一个batch的数据 
                                      fetch_list=[avg_cost, acc])          #fetch均方误差、准确率 
        test_costs.append(test_cost[0])                                    #记录每个batch的误差 
        test_accs.append(test_acc[0])                                      #记录每个batch的准确率 
    test_cost = (sum(test_costs) / len(test_costs))                        #计算误差平均值（误差和/误差的个数） 
    test_acc = (sum(test_accs) / len(test_accs))                           #计算准确率平均值（ 准确率的和/准确率的个数） 
    print('Test:%d, Cost:%0.5f, ACC:%0.5f' % (pass_id, test_cost, test_acc)) 
     
    #保存模型 
    model_save_dir = "/home/airobot/cat_dog/catdog.inference.model" 
    # 如果保存路径不存在就创建 
    if not os.path.exists(model_save_dir): 
        os.makedirs(model_save_dir) 
    fluid.io.save_inference_model(model_save_dir,['images'],[predict], exe,params_filename="mlp" + '-params',model_filename="mlp" + '-model',) 
print('训练模型保存完成！') 