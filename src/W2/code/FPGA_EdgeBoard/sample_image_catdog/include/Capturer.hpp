//
//  Capturer.hpp
//  asd
//
//  Created by 阿凡树 on 2018/5/31.
//  Copyright © 2018 ManGang. All rights reserved.
//

#ifndef Capturer_hpp
#define Capturer_hpp

#include <opencv2/opencv.hpp>

typedef int (*callback)(cv::Mat&);

class Capturer {
public:
    Capturer(callback call);
    void mainloop(void);
    void release(void);
    ~Capturer();
private:
    callback call;
    void errno_exit(const char *s);
    int xioctl(int fh, int request, void *arg);
    void process_image(void *p, int size);
    int read_frame(void);
    void stop_capturing(void);
    void start_capturing(void);
    void uninit_device(void);
    void init_read(unsigned int buffer_size);
    void init_mmap(void);
    void init_userp(unsigned int buffer_size);
    void init_device(void);
    void close_device(void);
    void open_device(void);
};

#endif /* Capturer_hpp */
