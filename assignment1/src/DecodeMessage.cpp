// DecodeMessage.cpp

#include "DecodeMessage.h"
#include <iostream>

// Default constructor
DecodeMessage::DecodeMessage() {
    // Nothing specific to initialize here
}

// Destructor
DecodeMessage::~DecodeMessage() {
    // Nothing specific to clean up
}


std::string DecodeMessage::decodeFromImage(const ImageMatrix& image, const std::vector<std::pair<int, int>>& edgePixels) {
    // extract least significant bit from edge pixel
    std::string binary_string;
    for (const std::pair<int, int>& edgePixel : edgePixels) {
        int pix_value = (int)image.get_data(edgePixel.first, edgePixel.second);
        binary_string += std::to_string(pix_value & 1);
    }

    // padding with zeros to form complete 7-bit bytes.
    int padding_num = 7 - (binary_string.length() % 7);
    if (padding_num != 7) {
        binary_string = std::string(padding_num, '0') + binary_string;
    }
    
    // converting binary to ASCII
    std::string decoded_message;
    for (int i = 0; i < binary_string.size(); i+=7){
        std::string binary_part = binary_string.substr(i, 7);
        int ascii_value = std::stoi(binary_part, 0, 2);

        if (ascii_value == 127) {
            ascii_value = 126;
        } else if (ascii_value <= 32) {
            ascii_value += 33;
        }

        decoded_message += char(ascii_value);
    }

    return decoded_message;
}

