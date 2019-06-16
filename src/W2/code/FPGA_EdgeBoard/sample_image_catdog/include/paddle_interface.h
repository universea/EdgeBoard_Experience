/***************************************************************************
*
* Copyright(c) 2018 Baidu.com, Inc. All Rights Reserved
*
**************************************************************************/
/**
* @file paddle_interface.h
* @author Jesse Qu (quchunyu@baidu.com)
* @date 2018-12-28
* @brief
*
**/

#ifndef PADDLE_EASYDL_PADDLE_INTERFACE_H
#define PADDLE_EASYDL_PADDLE_INTERFACE_H

#include <iostream>
#include <vector>
#include "paddle_include.h"

class Predictor {
public:
    Predictor();

    virtual ~Predictor() {};

    void init(const std::string& base_directory,
              const std::string& model_file_name, const std::string& param_file_name,
              std::vector<int> shape,
              const std::vector<std::string> output_names);

    bool predict(float* inputs,
                 std::vector<std::vector<float> >& outputs,
                 std::vector<std::vector<int> >& output_shapes);
private:

    std::string _base_directory;
    std::string _model_file_name;
    std::string _param_file_name;

    paddle_mobile::framework::DDim _input_shape;

    paddle_mobile::PaddleMobile<paddle_mobile::FPGA> _paddle_mobile;

    int _fd = -1;
};

#endif //PADDLE_EASYDL_PADDLE_INTERFACE_H
