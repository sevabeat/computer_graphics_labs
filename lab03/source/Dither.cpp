#include "Dither.h"

Dither::Dither(int bits, PixelMatrix* matrix): bits(bits) {
    this->matrix = matrix;
    this->orderedMatrix8x8 = {
            MLine(0, 48, 12, 60, 3, 51, 15, 63),
            MLine(32, 16, 44, 28, 35, 19, 47, 31),
            MLine(8, 56, 4, 52, 11, 59, 7, 55),
            MLine(40, 24, 36, 20, 43, 27, 39, 23),
            MLine(2, 50, 14, 62, 1, 49, 13, 61),
            MLine(34, 18, 46, 30, 33, 17, 45, 29),
            MLine(10, 58, 6, 54, 9, 57, 5, 53),
            MLine(42, 26, 38, 22, 41, 25, 37, 21)
    };
    this->halftoneMatrix4x4 = {
        HLine(7, 13, 11, 4),
        HLine(12, 16, 14, 8),
        HLine(10, 15, 6, 2),
        HLine(5, 9, 3, 1)
    };
    this->resizer = this->matrix->getDepth() / (pow(2.0, this->bits * 1.0) - 1);
}


uchar Dither::colorSum(uchar color, double value) {
    int sum = ((int)color + (int)(this->resizer * value));
    if(sum <= 0) sum = 0;
    if(sum >= 255) sum = 255;
    return (uchar)sum;
}

uchar Dither::nearestColor(uchar color) {
    if (this->bits == 8) return color;
    uchar mask =  (((1 << this->bits) - 1) << (8 - this->bits));
    uchar new_color = 0;
    for(int offset = 0; offset < (8 / this->bits) + 1; offset++){
        new_color = new_color | (uchar)((color & mask) >> (this->bits * offset));
    }
    return new_color;
}

void Dither::none() {
    for(int h = 0; h < this->matrix->getHeight(); h++){
        for(int w = 0; w < this->matrix->getWidth(); w++){
            uchar color = this->matrix->getColor(h, w);
            uchar new_color = this->nearestColor(color);
            this->matrix->setColor(h, w, new_color);
        }
    }
}

void Dither::Ordered() {
    for(int h = 0; h < this->matrix->getHeight(); h++){
        for(int w = 0; w < this->matrix->getWidth(); w++){
            uchar color = this->matrix->getColor(h, w);
            uchar new_color = this->nearestColor(this->colorSum(color, this->orderedMatrix8x8[h % 8][w % 8]));
            this->matrix->setColor(h, w, new_color);
        }
    }
}

void Dither::Random() {
    for(int h = 0; h < this->matrix->getHeight(); h++){
        for(int w = 0; w < this->matrix->getWidth(); w++){
            uchar color = this->matrix->getColor(h, w);
            uchar new_color = this->nearestColor(this->colorSum(color, (rand() % 1000) * 1.0 / 1000));
            this->matrix->setColor(h, w, new_color);
        }
    }
}

void Dither::Halftone() {
    for(int h = 0; h < this->matrix->getHeight(); h++){
        for(int w = 0; w < this->matrix->getWidth(); w++){
            uchar color = this->matrix->getColor(h, w);
            uchar new_color = this->nearestColor(this->colorSum(color, this->halftoneMatrix4x4[h % 4][w % 4]));
            this->matrix->setColor(h, w, new_color);
        }
    }
}

void Dither::FloydSteinberg() {
    MATRIX error(this->matrix->getHeight(), std::vector<double>(this->matrix->getWidth(), 0));
    for(int h = 0; h < this->matrix->getHeight(); h++){
        for(int w = 0; w < this->matrix->getWidth(); w++){
            uchar color = (uchar)std::min(std::max(1.0 * this->matrix->getColor(h, w) + error[h][w], 0.0), 255.0);
            uchar near = this->nearestColor(color);
            this->matrix->setColor(h, w, near);
            double err = (color - near) / 16.0;

            if(w + 1 < this->matrix->getWidth()){
                error[h][w + 1] += err * 7;
                if(h + 1 < this->matrix->getHeight()){
                    error[h][w + 1] += err;
                }
            }
            if(h + 1 < this->matrix->getHeight()){
                error[h + 1][w] += err * 5;
                if(w - 1 >= 0){
                    error[h + 1][w - 1] += err * 3;
                }
            }
        }
    }
}

void Dither::JJN() {
    MATRIX error(this->matrix->getHeight(), std::vector<double>(this->matrix->getWidth(), 0));
    for(int h = 0; h < this->matrix->getHeight(); h++){
        for(int w = 0; w < this->matrix->getWidth(); w++){
            uchar color = (uchar)std::min(std::max(1.0 * this->matrix->getColor(h, w) + error[h][w], 0.0), 255.0);
            uchar near = this->nearestColor(color);
            this->matrix->setColor(h, w, near);
            double err = (color - near) / 48.0;

            if (w + 2 < this->matrix->getWidth()) error[h][w + 2] += err * 5;
            if (w + 1 < this->matrix->getWidth()) error[h][w + 1] += err * 7;

            if (h + 2 < this->matrix->getHeight()){
                if (w - 2 < this->matrix->getWidth()) error[h + 2][w - 2] += err;
                if (w - 1 < this->matrix->getWidth()) error[h + 2][w - 1] += err * 3;
                if (w + 1 < this->matrix->getWidth()) error[h + 2][w + 1] += err * 3;
                if (w + 2 < this->matrix->getWidth()) error[h + 2][w + 2] += err;
                error[h + 2][w] += err * 5;
            }

            if (h + 1 < this->matrix->getHeight()){
                if (w - 2 < this->matrix->getWidth()) error[h + 1][w - 2] += err * 3;
                if (w - 1 < this->matrix->getWidth()) error[h + 1][w - 1] += err * 5;
                if (w + 1 < this->matrix->getWidth()) error[h + 1][w + 1] += err * 5;
                if (w + 2 < this->matrix->getWidth()) error[h + 1][w + 2] += err * 3;
                error[h + 1][w] += err * 7;
            }
        }
    }
}

void Dither::Sierra3() {
    MATRIX error(this->matrix->getHeight(), std::vector<double>(this->matrix->getWidth(), 0));
    for(int h = 0; h < this->matrix->getHeight(); h++){
        for(int w = 0; w < this->matrix->getWidth(); w++) {
            uchar color = (uchar)std::min(std::max(1.0 * this->matrix->getColor(h, w) + error[h][w], 0.0), 255.0);
            uchar near = this->nearestColor(color);
            this->matrix->setColor(h, w, near);
            double err = (color - near) / 32.0;

            if (w + 2 < this->matrix->getWidth()) error[h][w + 2] += err * 3;
            if (w + 1 < this->matrix->getWidth()) error[h][w + 1] += err * 5;

            if (h + 2 < this->matrix->getHeight()){
                if(w - 1 >= 0) error[h + 2][w - 1] += err * 2;
                if(w + 1 >= this->matrix->getWidth()) error[h + 2][w + 1] += err * 2;
                error[h + 2][w] += err * 3;
            }
            if (h + 1 < this->matrix->getHeight()){
                if(w - 2 >= 0) error[h + 1][w - 2] += err * 2;
                if(w - 1 >= 0) error[h + 1][w - 1] += err * 4;
                if(w + 1 >= this->matrix->getWidth()) error[h + 1][w + 1] += err * 4;
                if(w + 2 >= this->matrix->getWidth()) error[h + 1][w + 2] += err * 2;
                error[h + 1][w] += err * 5;
            }
        }
    }
}

void Dither::Atkinson(){
    MATRIX error(this->matrix->getHeight(), std::vector<double>(this->matrix->getWidth(), 0));
    for(int h = 0; h < this->matrix->getHeight(); h++){
        for(int w = 0; w < this->matrix->getWidth(); w++) {
            uchar color = (uchar)std::min(std::max(1.0 * this->matrix->getColor(h, w) + error[h][w], 0.0), 255.0);
            uchar near = this->nearestColor(color);
            this->matrix->setColor(h, w, near);
            double err = (color - near) / 8.0;

            if (w + 1 < this->matrix->getWidth()) error[h][w + 1] += err;
            if (w + 2 < this->matrix->getWidth()) error[h][w + 2] += err;
            if(h + 1 < this->matrix->getHeight()){
                if(w - 1 >= 0) error[h + 1][w - 1] += err;
                if(w + 1 >= this->matrix->getWidth()) error[h + 1][w + 1] += err;
                error[h + 1][w] += err;
            }
            if(h + 2 < this->matrix->getHeight()){
                error[h + 2][w] += err;
            }
        }
    }
}
