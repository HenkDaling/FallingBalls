#include "BitmapFontExtractor.h"
#include <iostream>
#include <fstream>
#include <sstream>

BitmapFontExtractor::BitmapFontExtractor(const Image& bitmapImage, const std::string& fontInfo)
    : image(bitmapImage) {
    parseFontInfo(fontInfo);
}

BitmapFontExtractor::~BitmapFontExtractor() {}

Image BitmapFontExtractor::extractChar(int charId) {
    for (const CharInfo& charInfo : charInfoList) {
        if (charInfo.id == charId) {
            // Implement the extraction logic based on charInfo from the image
            Image charImage;

            return charImage;
        }
    }

    return Image();
}

void BitmapFontExtractor::parseFontInfo(const string& fontInfo) {
    std::istringstream stream(fontInfo);
    std::string line;
    while (std::getline(stream, line)) {
        if (line.find("char ") == 0) {
            CharInfo charInfo;
            std::istringstream charStream(line.substr(5));

            charStream >> charInfo.id;
            charStream >> charInfo.x;
            charStream >> charInfo.y;
            charStream >> charInfo.width;
            charStream >> charInfo.height;
            charStream >> charInfo.xoffset;
            charStream >> charInfo.yoffset;
            charStream >> charInfo.xadvance;

            charInfoList.push_back(charInfo);
        }
    }
}
