#include "EncodeMessage.h"
#include <cmath>
#include <bitset>

// Default Constructor
EncodeMessage::EncodeMessage() {

}

// Destructor
EncodeMessage::~EncodeMessage() {

}   

//checking prime number. if it is, return true
bool EncodeMessage::check_prime(int num){
    if(num == 0 || num ==1){
        return false;
    }
    if(num == 2){
        return true;
    }
    for (int i = 2; i <= num / 2; i++){
        if (num % i == 0)
            return false;
    }

    return true;
}

// function to caluculate fibonacci number
int EncodeMessage::fibonacci(int index){
    if (index == 0 || index == 1){
        return index;
    }

    int first = 0;
    int second = 1;

    for (int i = 1; i <= index; i++) {
        int next = first + second;
        first = second;
        second = next;
    }

    return second;
}


// Function to encode a message into an image matrix
ImageMatrix EncodeMessage::encodeMessageToImage(const ImageMatrix &img,
 const std::string &message, const std::vector<std::pair<int, int>>& positions) {
    
    ImageMatrix img_encoded = img;
    std::string binary_string = "";

    // Character Transformation
    for (int i = 0; i < message.size(); i++){
        if(check_prime(i)){
            int temp_ascii = static_cast<int>(message[i]) + fibonacci(i);

            if (temp_ascii == 127) {
                temp_ascii = 126;
            } else if (temp_ascii <= 32) {
                temp_ascii += 33;
            }

            binary_string += char(temp_ascii);
        }
        else{
            binary_string += message[i];
        }
        
    }

    // Right Circular Shifting
    int shift = binary_string.size() / 2;
    binary_string = binary_string.substr(binary_string.size() - shift)
     + binary_string.substr(0, binary_string.size() - shift);


    // Message Embedding
    for (int i = 0; i < binary_string.size(); i++) {
        int row = positions[i].first;
        int col = positions[i].second;
        int pixel_value = (int)img_encoded.get_data(row, col);
        if((static_cast<int>(binary_string[i]) & 1) == 0){
            img_encoded.set_data(row, col, pixel_value | 1); 
        }
        else{
            img_encoded.set_data(row, col, pixel_value & -1);
        }
    }

    return img_encoded;
}
