#include "PixelMatrix.h"

PixelMatrix::PixelMatrix(uint width, uint height, uint pixelBytes, uchar depth):
width(width), height(height), pixelBytes(pixelBytes), depth(depth){
    this->_size = 1LL * this->width * this->height * this->pixelBytes;
    this->matrix = new PIXEL[this->size()];
    if(this->matrix == nullptr){
        throw std::bad_alloc();
    }
}

ull PixelMatrix::size(){
    return this->_size;
}
uint PixelMatrix::getWidth(){
    return this->width;
}
uint PixelMatrix::getHeight(){
    return this->height;
}
uint PixelMatrix::getDepth(){
    return this->depth;
}

ull PixelMatrix::getPosition(uint h, uint w, uint lineWidth) {
    if(lineWidth == -1) lineWidth = this->getWidth();
    return (1LL * h * lineWidth + w) * this->pixelBytes;
}
void PixelMatrix::inverseColors() {
    for(ull i = 0; i < this->size(); i++){
        this->matrix[i] = this->depth - this->matrix[i];
    }
}
void PixelMatrix::horizontalReflection(){
    ull pos;
    uint new_w;
    for(uint h = 0; h < this->getHeight(); h++){
        pos = h * this->getWidth() * this->pixelBytes * 1LL;
        for(uint w = 0; w < this->getWidth() / 2; w++){
            new_w = this->getWidth() - w - 1;
            for(int bytes = 0; bytes < this->pixelBytes; bytes++){
                std::swap(this->matrix[this->getPosition(h, w) + bytes],this->matrix[this->getPosition(h, new_w) + bytes]);
            }
        }
    }
}
void PixelMatrix::verticalReflection(){
    uint new_h;
    for(uint h = 0; h < this->getHeight() / 2; h++){
        for(uint w = 0; w < this->getWidth(); w++){
            new_h = this->getHeight() - h - 1;
            for(int bytes = 0; bytes < this->pixelBytes; bytes++){
                std::swap(this->matrix[this->getPosition(h, w) + bytes],this->matrix[this->getPosition(new_h, w) + bytes]);

            }
        }
    }
}
void PixelMatrix::rotate(char rotationType){
    uchar* tmp_matrix = new PIXEL[this->size()];
    for(int i = 0; i < this->size(); i++){
        tmp_matrix[i] = this->matrix[i];
    }
    uint new_w, new_h;
    for(int h = 0; h < this->getHeight(); h++){
        for(int w = 0; w < this->getWidth(); w++){
            if(rotationType == MATRIX_ROTATE_LEFT){
                new_h = this->getWidth() - w - 1;
                new_w = h;
            }else if(rotationType == MATRIX_ROTATE_RIGHT){
                new_h = w;
                new_w = this->getHeight() - h - 1;
            }else return;
            for(int bytes = 0; bytes < this->pixelBytes; bytes++){
                this->matrix[this->getPosition(new_h, new_w, this->getHeight()) + bytes] = tmp_matrix[this->getPosition(h, w) + bytes];
            }
        }
    }
    std::swap(this->height, this->width);
    delete[] tmp_matrix;
}

std::ifstream& operator>>(std::ifstream& in, PixelMatrix* pixelMatrix) {
    in.read((char*)pixelMatrix->matrix, pixelMatrix->size());
    if(in.gcount() != pixelMatrix->size()){
        throw ImageException("Image body size doesn't match header");
    }
    char EOFTest;
    if(in >> EOFTest && !in.eof()){
        throw ImageException("Image body size doesn't match header");
    }
    return in;
}

std::ofstream& operator<<(std::ofstream& out, PixelMatrix* pixelMatrix) {
    out << pixelMatrix->getWidth() << " " << pixelMatrix->getHeight() << std::endl;
    out << pixelMatrix->getDepth() << std::endl;

    out.write((char*)pixelMatrix->matrix, pixelMatrix->size());
    return out;
}
PixelMatrix::~PixelMatrix(){
    delete[] this->matrix;
}