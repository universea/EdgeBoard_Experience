// #include "fpga_cv.hpp"


// using half_float::half;


// void fpga_resize(float* input, int input_width, int input_height, int input_channel,
// 	uint8_t* output, int output_width, int output_height) {

// 	paddle_mobile::fpga::InplaceArgs inplace_args = {
//         .relu_enable  = 0,
//         .power_enable = 0,
//     };
//     paddle_mobile::fpga::ConfigInplace(inplace_args);

//     paddle_mobile::fpga::ImageInputArgs input_args = {nullptr};
//     input_args.address = nullptr;
//     input_args.scale_address = nullptr;

//     half* input_image_address = (half *) paddle_mobile::fpga::fpga_malloc(input_width * 
//     	input_height * input_channel * sizeof(half));
//    	int index = 0;

//    	for (int i = 0; i < input_width * input_height * input_channel; i++) {

//    		input_image_address[i] = input[i];
//    	}


//     paddle_mobile::fpga::ResizeArgs resize_args = {0};
   
//     resize_args.input_width = input_width;
//     resize_args.input_height = input_height;
//     resize_args.image_channel = input_channel;
//     resize_args.output_width = output_width;
//     resize_args.output_height = output_height;
//     float height_ratio = input_width * 1.0f / output_width;
//     float width_ratio = input_height * 1.0f / output_height;
//     resize_args.height_ratio = *(uint32_t*)(&height_ratio);
//     resize_args.width_ratio = *(uint32_t*)(&width_ratio);

//     // resizeArgs.output_scale_address = scale_;
//     int output_size = output_width * resize_args.output_height * input_channel;
//     half * fpga_output = (half *) paddle_mobile::fpga::fpga_malloc(output_size * sizeof(half));
//     resize_args.input_image_address = input_image_address;
//     resize_args.output_image_address = fpga_output;

//     memset(fpga_output, 0, output_size * sizeof(half));
//     paddle_mobile::fpga::fpga_flush(input_image_address, input_width * input_height * input_channel* sizeof(half));
//     paddle_mobile::fpga::fpga_flush(resize_args.output_image_address, output_size * sizeof(half));
//     int ret = paddle_mobile::fpga::compute_fpga_resize(resize_args);
//     std::cout << "compute_fpga_resize ret：" << ret << std::endl;
//     if (ret == 0){
//         paddle_mobile::fpga::fpga_invalidate(resize_args.output_image_address, output_size * sizeof(half));
//     }

//     // uchar* output_img = (uchar*)malloc(output_size * sizeof(uchar));
//     for (int i = 0; i < output_size; i++) {
//         output[i] = fpga_output[i];
//     }

// }