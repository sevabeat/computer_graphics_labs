#include "PNM.h"

PNM::PNM(): loaded(false){}

void PNM::read(std::string filePath){
    std::ifstream fileStream(filePath, std::ios::binary);
    if(!fileStream) throw ImageException("Input file is not exist");
    fileStream >> this->type;
    if(!fileStream.good()) throw ImageException("Input file is not valid");

    uint pixelBytes;
    if(this->type == "P5"){
        pixelBytes = 1;
    }else if(this->type == "P6"){
        pixelBytes = 3;
    }else{
        throw ImageException("File format is not supported");
    }
    std::string comment;
    int widthOffset = (int)fileStream.tellg();
    fileStream.seekg(1, std::ios::cur);

    while(true){
        getline(fileStream, comment);
        if(comment[0] == '#'){
            this->comments.push_back(comment);
            widthOffset = (uint)fileStream.tellg();
        }else break;
    }
    fileStream.seekg(widthOffset, std::ios::beg);
    int width, height, depth;
    fileStream >> width >> height >> depth;
    if(!fileStream.good()) throw ImageException("Input file is not valid");

    if(width >= 0 && height >= 0 && depth >= 0 && depth <= 255) {
        fileStream.seekg(1, std::ios::cur);
        this->matrix = new PixelMatrix(width, height, pixelBytes, depth);
        fileStream >> this->matrix;
        this->loaded = true;
    }else{
        throw ImageException("Input file is not valid");
    }
    fileStream.close();
}
void PNM::write(std::string filePath){
    if(this->loaded) {
        std::ofstream fileStream(filePath, std::ios::binary);
        fileStream << this->type << std::endl;
        for (std::string &comment : this->comments) {
            fileStream << comment << std::endl;
        }
        fileStream << this->matrix;
        if(!fileStream.good()) throw ImageException("Write error");
        fileStream.close();
    }else throw ImageException("Image is not loaded");
}
void PNM::inverseColors(){
    this->matrix->inverseColors();
}
void PNM::horizontalReflection(){
    this->matrix->horizontalReflection();
}
void PNM::verticalReflection(){
    this->matrix->verticalReflection();
}
void PNM::rotateLeft90(){
    this->matrix->rotate(MATRIX_ROTATE_LEFT);
}
void PNM::rotateRight90(){
    this->matrix->rotate(MATRIX_ROTATE_RIGHT);
}
PNM::~PNM(){
    delete this->matrix;
}