#pragma once

#include <sys/time.h>
// timer for benchmark time useage

class Timer
{
    public:
        double _start, _end;

        void tic()
        {
            struct timeval tp;
            gettimeofday(&tp, NULL);
            _start = tp.tv_sec;
            _end = tp.tv_usec;
        }

        double toc()
        {
            struct timeval tp;
            gettimeofday(&tp, NULL);
            return(((double)(tp.tv_sec - _start)) + (tp.tv_usec - _end) / 1000000.0);
        }
};

