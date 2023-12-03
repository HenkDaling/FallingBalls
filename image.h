#ifndef IMAGE_H
#define IMAGE_H

class Image {
public:
    unsigned int width;
    unsigned int height;
    unsigned int bytesPerPixel;
    unsigned char* pixelData;

    Image(){};

    template <typename T> Image(const T& image) {
        width = image.width;
        height = image.height;
        bytesPerPixel = image.bytes_per_pixel;

        size_t dataSize = width * height * bytesPerPixel;

        // Allocate memory for pixel data
        pixelData = new unsigned char[dataSize];

        // Copy the pixel data
        for (size_t i = 0; i < dataSize; ++i) {
            pixelData[i] = image.pixel_data[i];
        }
    }

    ~Image() {
        
        pixelData = nullptr;       
    }

    void displayInfo() const;
};

#endif // IMAGE_H