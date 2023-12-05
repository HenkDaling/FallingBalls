#ifndef BITMAP_FONT_EXTRACTOR_H
#define BITMAP_FONT_EXTRACTOR_H

#include "Image.h"
#include <vector>
#include <string>

using namespace std;

class BitmapFontExtractor {
public:
    struct CharInfo {
        int id;
        int x;
        int y;
        int width;
        int height;
        int xoffset;
        int yoffset;
        int xadvance;
    };

private:
    Image image;
    vector<CharInfo> charInfoList;

public:
    BitmapFontExtractor(const Image& bitmapImage, const string& fontInfo);
    ~BitmapFontExtractor();

    Image extractChar(int charId);

private:
    void parseFontInfo(const string& fontInfo);
};

#endif // BITMAP_FONT_EXTRACTOR_H
