/*
Authors: Henk Daling, Mark Pattillo, Griffin Roe,
    Joshua Gilpin, David Sobernheim
Assignment Title: Falling Balls
Assignment Description: Create the falling balls game
Due Date: 12/6/23
Date Created: 11/15/23
Date Last Modified: 12/6/23
*/

#include "image.h"
#include <iostream>

template <typename T> Image::Image(const T& image) {
        width = image.width;
        height = image.height;
        bytesPerPixel = image.bytes_per_pixel;

        // Calculate the size of pixel data
        size_t dataSize = width * height * bytesPerPixel;

        // Allocate memory for pixel data
        pixelData = new unsigned char[dataSize];

        // Copy the pixel data
        for (size_t i = 0; i < dataSize; ++i) {
            pixelData[i] = image.pixel_data[i];
        }
    }

Image::~Image() {
    // Deallocate memory for pixel data
    delete[] pixelData;
}

void Image::displayInfo() const {
    std::cout << "Image Information:\n";
    std::cout << "Width: " << width << "\n";
    std::cout << "Height: " << height << "\n";
    std::cout << "Bytes per Pixel: " << bytesPerPixel << "\n";
    std::cout << "Pixel Data: " << pixelData << "\n";
}
