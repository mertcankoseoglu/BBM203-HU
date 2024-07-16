#include "ImageProcessor.h"

ImageProcessor::ImageProcessor() {

}

ImageProcessor::~ImageProcessor() {

}


std::string ImageProcessor::decodeHiddenMessage(const ImageMatrix &img) {
    ImageSharpening sharpen_opt;
    ImageMatrix sharpend_img = sharpen_opt.sharpen(img, 2);

    EdgeDetector edge_detector;
    std::vector<std::pair<int, int>> edge_pixels = edge_detector.detectEdges(sharpend_img);
    DecodeMessage decoded;
    
    std::string hidden_msg = decoded.decodeFromImage(sharpend_img, edge_pixels);
    return hidden_msg;
}

ImageMatrix ImageProcessor::encodeHiddenMessage(const ImageMatrix &img, const std::string &message) {
    ImageSharpening sharpen_opt;
    ImageMatrix sharpend_img = sharpen_opt.sharpen(img, 2);

    EdgeDetector edge_detector;
    std::vector<std::pair<int, int>> edge_pixels = edge_detector.detectEdges(sharpend_img);

    EncodeMessage encoded;
    ImageMatrix encoded_img = encoded.encodeMessageToImage(img, message, edge_pixels);
    return encoded_img;
}
