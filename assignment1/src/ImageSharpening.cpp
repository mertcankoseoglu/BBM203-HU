#include "ImageSharpening.h"

// Default constructor
ImageSharpening::ImageSharpening() {
    // Allocate memory for the kernel
    blurring_kernel = new double*[3];
    for (int i = 0; i < 3; ++i) {
        blurring_kernel[i] = new double[3];
        for (int j = 0; j < 3; j++) {
            blurring_kernel[i][j] = (double)1/9;
        }
    }
}

ImageSharpening::~ImageSharpening(){
    // Deallocate memory for kernel
    if (blurring_kernel != nullptr) {
        for (int i = 0; i < 3; ++i) {
            delete[] blurring_kernel[i];
        }
        delete[] blurring_kernel;
    }
}

ImageMatrix ImageSharpening::sharpen(const ImageMatrix& input_image, double k) {
    Convolution convolution(blurring_kernel, 3, 3, 1, true);
    ImageMatrix blurred_image = convolution.convolve(input_image);
    ImageMatrix sharpen_image = input_image + (input_image - blurred_image)*k;

    for(int i=0; i < sharpen_image.get_height(); i++){
        for (int j = 0; j < sharpen_image.get_width(); j++){
            if(sharpen_image.get_data(i, j) > 255){
                sharpen_image.set_data(i, j, 255);
            }
            else if(sharpen_image.get_data(i, j) < 0){
                sharpen_image.set_data(i, j, 0);
            }   
        } 
    }

    return sharpen_image;
}
