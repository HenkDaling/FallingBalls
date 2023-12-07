/*
Authors: Henk Daling, Mark Pattillo, Griffin Roe,
    Joshua Gilpin, David Sobernheim
Assignment Title: Falling Balls
Assignment Description: Create the falling balls game
Due Date: 12/6/23
Date Created: 11/15/23
Date Last Modified: 12/6/23
*/

#ifndef IMAGE_H
#define IMAGE_H

#include <cstddef>
#include <cstring>

class Image {
public:
    unsigned int width;
    unsigned int height;
    unsigned int bytesPerPixel;
    unsigned char* pixelData;

    Image() : width(0), height(0), bytesPerPixel(0), pixelData(nullptr) {}
    
    Image(unsigned int width, unsigned int height, unsigned int bpp, const unsigned char* srcPixelData) 
        : width(width), height(height), bytesPerPixel(bpp) {

        size_t dataSize = width * height * bytesPerPixel;
        pixelData = new unsigned char[dataSize];
        std::memcpy(pixelData, srcPixelData, dataSize);

    }

    template <typename T> Image(const T& image) {
        width = image.width;
        height = image.height;
        bytesPerPixel = image.bytes_per_pixel;

        size_t dataSize = width * height * bytesPerPixel;

        pixelData = new unsigned char[dataSize];

        for (size_t i = 0; i < dataSize; ++i) {
            pixelData[i] = image.pixel_data[i];
        }
    }

    ~Image() {
           
    }   

    void displayInfo() const;
};

#endif // IMAGE_H
