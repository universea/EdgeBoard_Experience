#ifndef _FPGA_CV__H_
#define _FPGA_CV__H_

#include <stdlib.h>
#include "fpga/KD/half.h"
#include "fpga/KD/api.h"
#include "fpga/KD/fpga_common.h"

void fpga_resize(float* input, int input_width, int input_height, int input_channel,
	uint8_t* output, int output_width, int output_height);



#endif
