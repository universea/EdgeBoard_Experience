/* Copyright (c) 2018 PaddlePaddle Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

#include <iostream>
#include "io/paddle_inference_api.h"

#include "math.h"
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <ostream>
#include <fstream>
#include <iomanip>
#include <typeinfo>
#include <typeindex>
#include <vector>

#include<ctime>

#include "fpga/KD/float16.hpp"
#include "fpga/KD/llapi/zynqmp_api.h"

using namespace paddle_mobile;

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
using namespace cv;

cv::Mat sample_float;

static std::vector<std::string> label_list(10);

void readImage(std::string filename, float* buffer) {
  Mat img = imread(filename);
  if (img.empty()) {
      std::cerr << "Can't read image from the file: " << filename << std::endl;
      exit(-1);
  }
  
  Mat img2;
  resize(img, img2, Size(32,32));

  img2.convertTo(sample_float, CV_32FC3);

  int index = 0;
  for (int row = 0; row < sample_float.rows; ++row) {
    float* ptr = (float*)sample_float.ptr(row);
    for (int col = 0; col < sample_float.cols; col++) {
        float* uc_pixel = ptr;
        // uc_pixel[0] -= 102;
        // uc_pixel[1] -= 117;
        // uc_pixel[1] -= 124;
        float r = uc_pixel[0];
        float g = uc_pixel[1];
        float b = uc_pixel[2];     

        buffer[index] = b / 255.0;
        buffer[index + 1] = g  / 255.0;
        buffer[index + 2] = r  / 255.0;

        // sum += a + b + c;
        ptr += 3;
        // DLOG << "r:" << r << " g:" << g << " b:" << b;
        index += 3;
    }
  }
  // return sample_float;
}


PaddleMobileConfig GetConfig() {
  PaddleMobileConfig config;
  config.precision = PaddleMobileConfig::FP32;
  config.device = PaddleMobileConfig::kFPGA;
  // config.model_dir = "../models/mobilenet/";
  config.prog_file = "../model/mlp/model";
  config.param_file = "../model/mlp/params";
  config.thread_num = 4;
  return config;
}

int main() {
  clock_t startTime,endTime;
  
  zynqmp::open_device();
  std::cout << " open_device success " << std::endl;
  PaddleMobileConfig config = GetConfig();
  std::cout << " GetConfig success " << std::endl;
  auto predictor =
      CreatePaddlePredictor<PaddleMobileConfig,
                            PaddleEngineKind::kPaddleMobile>(config);
  std::cout << " predictor success " << std::endl;

  startTime = clock();//计时开始

  float data[1 * 3 * 32 * 32] = {1.0f};
  readImage("../image/bird.jpg", data);
  
  endTime = clock();//计时结束
  std::cout << "The run time of image process is: " <<(double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << std::endl;
  
  PaddleTensor tensor;
  tensor.shape = std::vector<int>({1, 3, 32, 32});
  tensor.data = PaddleBuf(data, sizeof(data));
  tensor.dtype = PaddleDType::FLOAT32;
  std::vector<PaddleTensor> paddle_tensor_feeds(1, tensor);

  PaddleTensor tensor_out;
  tensor_out.shape = std::vector<int>({});
  tensor_out.data = PaddleBuf();
  tensor_out.dtype = PaddleDType::FLOAT32;
  std::vector<PaddleTensor> outputs(1, tensor_out);

  std::cout << " before predict " << std::endl;

  for(int i=0;i<10000;i++)
    predictor->Run(paddle_tensor_feeds, &outputs);

  std::cout << " after predict " << std::endl;
  //  assert();

  endTime = clock();//计时结束
  std::cout << "The run time of predict is: " <<(double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << std::endl;


  float* data_o = static_cast<float*>(outputs[0].data.data());
  for (size_t j = 0; j < outputs[0].data.length() / sizeof(float); ++j) {
     std::cout << "output[" << j << "]: " << data_o[j] << std::endl;
  }

  int index = 0;
  float max = 0.0;
  for (int i = 0;i < 10; i++) {
      float val = data_o[i];
      if (val > max) {
          max = val > max ? val : max;
          index = i;
      }
  }

  label_list = {"airplane", "automobile", "bird", "cat", "deer", "dog", "frog", "horse", 
        "ship", "truck" };
  std::cout << "Result" << " is " << label_list[index] << std::endl;

  return 0;
}
