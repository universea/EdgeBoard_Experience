# 导入需要的包 nump: python第三方库，用于进行科学计算 PIL : Python Image Library,python,python第三方图像处理库 
# matplotlib:python的绘图库  
# pyplot:matplotlib的绘图框架  
 
import numpy as np 
from PIL import Image 
import paddle.fluid as fluid 
import matplotlib.pyplot as plt 
 
# 使用CPU进行训练 
place = fluid.CPUPlace() 
# 定义一个executor 
infer_exe = fluid.Executor(place) 
inference_scope = fluid.core.Scope()#要想运行一个网络，需要指明它运行所在的域，确切的说： exe.Run(&scope) 。 
model_save_dir = "/home/airobot/cat_dog/catdog.inference.model" 
 
def load_image(file): 
        #打开图片 
        im = Image.open(file) 
        #将图片调整为跟训练数据一样的大小  32*32，设定ANTIALIAS，即抗锯齿.resize是缩放 
        im = im.resize((32, 32), Image.ANTIALIAS) 
        #建立图片矩阵 类型为float32 
        im = np.array(im).astype(np.float32) 
        #矩阵转置  
        im = im.transpose((2, 0, 1))  # CHW，因为输入图像格式为[N，C，H，W] 
        #将像素值从【0-255】转换为【0-1】 
        im = im / 255.0 
 
#         print(im)        # 保持和之前输入image维度一致 
        im = np.expand_dims(im, axis=0) 
        print('im_shape的维度：',im.shape) 
        return im 
#fluid.scope_guard修改全局/默认作用域（scope）, 运行时中的所有变量都将分配给新的scope 
with fluid.scope_guard(inference_scope): 
    #获取训练好的模型 
    #从指定目录中加载 推理model(inference model) 
    [inference_program, # 预测用的program 
     feed_target_names, # 是一个str列表，它包含需要在推理 Program 中提供数据的变量的名称。  
     fetch_targets] = fluid.io.load_inference_model(model_save_dir,#fetch_targets：是一个 Variable 列表，从中我们可以得到推断结果。 
                                                    infer_exe,
                                                    params_filename ='mlp-params', 
	                                                model_filename='mlp-model')     #infer_exe: 运行 inference model的 executor 
    
    #获取推测数据，可以分别用猫和狗的图片进行预测 
#     img = load_image( '/home/aistudio/data/data2587/cat .jpg') 
    img = load_image( '/home/airobot/cat_dog/cat.jpg') 
 
    results = infer_exe.run(inference_program,                 #运行预测程序 
                            feed={feed_target_names[0]: img},  #喂入要预测的img 
                            fetch_list=fetch_targets)          #得到推测结果 
    print('results',results) 
    label_list = [ 
        "airplane", "automobile", "bird", "cat", "deer", "dog", "frog", "horse", 
        "ship", "truck" 
        ] 
    print("infer results: %s" % label_list[np.argmax(results[0])]) 
    img = Image.open('/home/airobot/cat_dog/cat.jpg') 
    plt.imshow(img)   #根据数组绘制图像 
    plt.show()        #显示图像 