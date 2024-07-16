#include <iostream>
#include "Convolution.h"

// Default constructor 
Convolution::Convolution() : kernel(nullptr), kernelHeight(0), kernelWidth(0), stride(0), padding(false) {}

// Parametrized constructor for custom kernel and other parameters
Convolution::Convolution(double** customKernel, int kh, int kw, int stride_val, bool pad) : kernelHeight(kh), kernelWidth(kw), 
stride(stride_val), padding(pad){
    // Allocate memory for the kernel
    kernel = new double*[kernelHeight];
    for (int i = 0; i < kernelHeight; ++i) {
        kernel[i] = new double[kernelWidth];
        for (int j = 0; j < kernelWidth; j++) {
            kernel[i][j] = customKernel[i][j];
        }
    }
}

// Destructor
Convolution::~Convolution() {
    // Deallocate memory for kernel
    if (kernel != nullptr) {
        for (int i = 0; i < kernelHeight; ++i) {
            delete[] kernel[i];
        }
        delete[] kernel;
    }
}

// Copy constructor
Convolution::Convolution(const Convolution &other) : kernelHeight(other.kernelHeight), kernelWidth(other.kernelWidth),
 stride(other.stride), padding(other.padding){
     // Allocate memory for the kernel and copy data
    kernel = new double*[kernelHeight];
    for (int i = 0; i < kernelHeight; ++i) {
        kernel[i] = new double[kernelWidth];
        for (int j = 0; j < kernelWidth; ++j) {
            kernel[i][j] = other.kernel[i][j];
        }
    }
}

// Copy assignment operator
Convolution& Convolution::operator=(const Convolution &other) {
    if (this == &other) {
        return *this; // self-assignment check
    }

    // Deallocate old memory
    if (kernel != nullptr) {
        for (int i = 0; i < kernelHeight; ++i) {
            delete[] kernel[i];
        }
        delete[] kernel;
    }

    // Copy from other
    kernelHeight = other.kernelHeight;
    kernelWidth = other.kernelWidth;
    stride = other.stride;
    padding = other.padding;
    kernel = new double*[kernelHeight];
    for (int i = 0; i < kernelHeight; ++i) {
        kernel[i] = new double[kernelWidth];
        for (int j = 0; j < kernelWidth; ++j) {
            kernel[i][j] = other.kernel[i][j];
        }
    }
    return *this;
}

// Convolve Function: Responsible for convolving the input image with a kernel and return the convolved image.
ImageMatrix Convolution::convolve(const ImageMatrix& input_image) const {
    int inputHeight = input_image.get_height();
    int inputWidth = input_image.get_width();
    int paddingInt = padding ? 1 : 0;
    int conHeight = (inputHeight - kernelHeight + 2 * paddingInt) / stride + 1; // output height formula
    int conWidth = (inputWidth - kernelWidth + 2 * paddingInt) / stride + 1;    // output width formula

    // new ImageMatrix object for convolved image
    ImageMatrix convolvedImage(conHeight, conWidth);

    // 2D vector to hold padding image with 0 padding of size 1
    std::vector<std::vector<double> > paddedVec(inputHeight + 2, std::vector<double>(inputWidth + 2, 0.0));

    // add the values of input image in the middle of padding vector
    for (int i = 0; i < inputHeight; i++) {
        for (int j = 0; j < inputWidth; j++) {
            paddedVec[i + 1][j + 1] = input_image.get_data(i, j);
        }
    }

    // convolve the paddingvector(if padding is true) or input image 
    for (int i = 0; i < conHeight; i++) {
        for (int j = 0; j < conWidth; j++) {
            double sum = 0.0;
            for (int m = 0; m < kernelHeight; m++) {
                for (int n = 0; n < kernelWidth; n++) {
                    if(padding){
                        sum += paddedVec[i*stride + m][j*stride + n] * kernel[m][n];
                    } 
                    else{
                        sum += input_image.get_data(i*stride + m, j*stride + n) * kernel[m][n];
                    }   
                }
            }
            convolvedImage.set_data(i, j, sum);
        }
    }
    
    return convolvedImage;
}
