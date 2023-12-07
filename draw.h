/*
Authors: Henk Daling, Mark Pattillo, Griffin Roe,
    Joshua Gilpin, David Sobernheim
Assignment Title: Falling Balls
Assignment Description: Create the falling balls game
Due Date: 12/6/23
Date Created: 11/15/23
Date Last Modified: 12/6/23
*/

#ifndef DRAW_H
#define DRAW_H

#include <cmath>
#include "SDL_Plotter.h"
#include <algorithm>
#include "vect.h"
#include "image.h"

#pragma once

class draw
{
public:
    draw();
    ~draw();

static color convertToColor(Uint32 c) {
    color result;
    Uint8 r,g,b;

    Uint8 alpha = (c >> 24) & 0xFF;
    r = (c >> 16) & 0xFF;
    g = (c >> 8) & 0xFF;
    b = c & 0xFF;

    result.R = r;
    result.G = g;
    result.B = b;

    return result;
}


static void drawLine(int x1, int y1, int x2, int y2, SDL_Plotter &g,color c) {

    bool endOfLine = false;
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;

    int err = dx - dy;

    while (!endOfLine) {
        g.plotPixel(x1, y1,c.R,c.G,c.B);

        if (x1 == x2 && y1 == y2) {
            endOfLine = true;
        }
        else{
            int e2 = 2 * err;

            if (e2 > -dy) {
                err -= dy;
                x1 += sx;
            }

            if (e2 < dx) {
                err += dx;
                y1 += sy;
            }
        }
    }
}

static void drawOutline(vector<point2D> points, SDL_Plotter &g,color c) {

    for (size_t i = 0; i < points.size() - 1; ++i) {
        if (points.at(i).x >= 0 && points.at(i).x < g.getCol() && points.at(i).y >= 0 && points.at(i).y < g.getRow()) {
            drawLine(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y, g, c);
        }
    }
    drawLine(points.back().x, points.back().y, points.front().x, points.front().y, g, c);


}

static void drawFillPolygon(vector<point2D> points, SDL_Plotter &g,color c) {

    double minY = points[0].y, maxY = points[0].y;
    for (point2D& point : points) {
        minY = std::min(minY, point.y);
        maxY = std::max(maxY, point.y);
    }

    for (double y = minY; y <= maxY; ++y) {
        std::vector<double> intersections;

        // Find intersections
        for (size_t i = 0; i < points.size(); ++i) {
            const point2D& p1 = points[i];
            const point2D& p2 = points[(i + 1) % points.size()];

            if ((p1.y <= y && p2.y > y) || (p2.y <= y && p1.y > y)) {
                // Edge crosses the scanline
                double x = p1.x + (y - p1.y) * (p2.x - p1.x) / (p2.y - p1.y);
                intersections.push_back(x);
            }
        }

        std::sort(intersections.begin(), intersections.end());

        for (size_t i = 0; i < intersections.size(); i += 2) {
            double x1 = intersections[i];
            double x2 = intersections[i + 1];
            drawLine(x1, y, x2, y, g, c);
        }
    }
}


static void drawImagePixels(point2D position, Image &img, SDL_Plotter &g, double angle = 0, point2D pivot = point2D(), color DontDraw = color(255,0,255)){
    int index = 0;
    point2D translated;

    double drawX,drawY;
    int rd;
    int gn;
    int bl;

    for (double y = position.y; y < position.y + img.height; ++y) {
        drawY = y;
        for (double x = position.x; x < position.x + img.width; ++x) {
            drawX = x;

            if(angle != 0){
                translated = point2D(x, y);
                translated = Vector2D::rotate(angle, translated, pivot);
                drawX = translated.x;
                drawY = translated.y;
            }
            if (drawX >= 0 && drawX < g.getCol() && drawY >= 0 && drawY < g.getRow()) {
                rd = img.pixelData[index++];
                gn = img.pixelData[index++];
                bl = img.pixelData[index++];

                if(img.bytesPerPixel == 4){ index++; }

                if(!(color(rd,gn,bl) == DontDraw)){ //SDL PLOTTER DOES NOT PROVIDE ALPHA CHANNEL
                    if(img.bytesPerPixel == 4){
                        g.plotPixel(drawX, drawY, rd, gn, bl);
                    }
                    else{
                        g.plotPixel(drawX, drawY, rd, gn, bl);
                    }
                }

            } else {
                index += (img.bytesPerPixel == 4) ? 4 : 3;
            }
        }
    }

}

static void drawText(point2D position, string text, SDL_Plotter &g, double angle = 0, point2D pivot = point2D()){
    //change

    //g.plotText(position.x, position.y, text);
}

private:

};

#endif
