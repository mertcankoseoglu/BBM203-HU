// EdgeDetector.cpp

#include "EdgeDetector.h"
#include <cmath>

#include "EdgeDetector.h"
#include <cmath>

// Default constructor
EdgeDetector::EdgeDetector() {
    gradient_x = new double*[3]{
        new double[3]{-1,0,1},
        new double[3]{-2, 0, 2},
        new double[3]{-1, 0, 1}
    };
    gradient_y = new double*[3]{
        new double[3]{-1, -2, -1},
        new double[3]{0, 0, 0},
        new double[3]{1, 2, 1}
    };
    
}

// Destructor
EdgeDetector::~EdgeDetector() {
    for (int i = 0; i < 3; i++) {
        delete[] gradient_x[i];
        delete[] gradient_y[i];
    }
    delete[] gradient_x;
    delete[] gradient_y;
}

// Detect Edges using the given algorithm
std::vector<std::pair<int, int>> EdgeDetector::detectEdges(const ImageMatrix& input_image) {
    Convolution horizantal(gradient_x, 3, 3, 1, true);
    Convolution vertical(gradient_y, 3, 3, 1, true);

    ImageMatrix img_x = horizantal.convolve(input_image);
    ImageMatrix img_y = vertical.convolve(input_image);

    ImageMatrix img_magnitude(input_image.get_height(), input_image.get_width());

    double total_magnitude = 0.0;
    double total = 0.0;

    for (int i = 0; i < input_image.get_height(); i++) {
        for (int j = 0; j < input_image.get_width(); j++) {
            double i_x = img_x.get_data(i, j);
            double i_y = img_y.get_data(i, j);
            double g_magnitude = std::sqrt(i_x * i_x + i_y * i_y);
            img_magnitude.set_data(i, j, g_magnitude);
            total_magnitude += img_magnitude.get_data(i, j);
            total++;
        }
    }

    double threshold = total_magnitude / total;

    std::vector<std::pair<int, int>> edge_pixels;

    for (int i = 0; i < img_magnitude.get_height(); i++) {
        for (int j = 0; j < img_magnitude.get_width(); j++) {
            if (img_magnitude.get_data(i, j) > threshold) {
                edge_pixels.emplace_back(std::make_pair(i, j));
            }
        }
    }

    return edge_pixels;
}
