#ifndef SSD_DETECT_H
#define SSD_DETECT_H

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <string>
#include "paddle_interface.h"

typedef struct DetectInfo {
    float left;
    float top;
    float width;
    float height;
    float score;
    int label;
} DetectInfo;

class SSD_Detect {

public:
	SSD_Detect();
    ~SSD_Detect();
    void predect(cv::Mat& Mat, std::vector<DetectInfo>& detectInfos);

private:
    Predictor predictor;
    int input_width;
    int input_height;
    int input_channel;
    cv::Mat mean_;

    void setMean(const std::string& mean_file, const std::string& mean_value);
    void preprocess(const cv::Mat& img, cv::Mat preprocessMat);
    
};


#endif