#ifndef  __FN2MAT_H_
#define  __FN2MAT_H_

#include<iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"


class Fn2mat {

public:
    Fn2mat();

    Fn2mat(char *fn, cv::Mat &new_mat) {
        _frameno = (char *) malloc(4 * 8);
        memcpy(_frameno, fn, 32);    //？溢出

        _mat = new_mat;
    }

    Fn2mat(const Fn2mat &fn2mat) {
        _frameno = (char *) malloc(4 * 8);
        memcpy(_frameno, fn2mat._frameno, 32);

        _mat = fn2mat._mat;
    }

    char *get_fn() {
        return _frameno;
    }

    cv::Mat &get_mat() {
        return _mat;
    }

    ~Fn2mat() {
        free(_frameno);
    }

private:
    char *_frameno;
    cv::Mat _mat;
};


#endif