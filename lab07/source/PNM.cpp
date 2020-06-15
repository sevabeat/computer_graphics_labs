#include "PNM.h"

PNM::PNM(): loaded(false), matrix(nullptr){}

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

void PNM::ContrastAdaptiveSharpening(double sharpness) {
    PixelMatrix* new_matrix = new PixelMatrix(this->matrix->getWidth(), this->matrix->getHeight(), this->matrix->pixelBytes, this->matrix->getDepth());


    //Заполняю рамку шириной 1px в новой матрице
    for(int w = 0; w < this->matrix->getWidth(); w++){
        for(int i = 0; i < this->matrix->pixelBytes; i++) {
            new_matrix->matrix[new_matrix->getPosition(0, w) + i] = this->matrix->matrix[this->matrix->getPosition(0, w) + i];
            new_matrix->matrix[new_matrix->getPosition(this->matrix->getHeight() - 1, w) + i]
            = this->matrix->matrix[this->matrix->getPosition(this->matrix->getHeight() - 1, w) + i];
        }
    }

    for(int h = 1; h < this->matrix->getHeight() - 1; h++){
        for(int i = 0; i < this->matrix->pixelBytes; i++) {
            new_matrix->matrix[new_matrix->getPosition(h, 0) + i] = this->matrix->matrix[this->matrix->getPosition(h, 0) + i];
            new_matrix->matrix[new_matrix->getPosition(h, this->matrix->getWidth() - 1) + i]
                    = this->matrix->matrix[this->matrix->getPosition(h, this->matrix->getWidth() - 1) + i];
        }
    }
    for(int h = 1; h < this->matrix->getHeight() - 1; h++){
        for(int w = 1; w < this->matrix->getWidth() - 1; w++){
            /*
             *
             * | 0 | 1 | 2 |
             * |---|---|---|
             * | 3 | 4 | 5 |
             * |---|---|---|
             * | 6 | 7 | 8 |
             *
             * */

            COLOR_LIST local(9);
            for(int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    if(this->type == "P6"){
                        local[(dy + 1) * 3 + dx + 1].R = PNM::toIntensive(this->matrix->matrix[this->matrix->getPosition(h + dy, w + dx) + 0]);
                        local[(dy + 1) * 3 + dx + 1].G = PNM::toIntensive(this->matrix->matrix[this->matrix->getPosition(h + dy, w + dx) + 1]);
                        local[(dy + 1) * 3 + dx + 1].B = PNM::toIntensive(this->matrix->matrix[this->matrix->getPosition(h + dy, w + dx) + 2]);
                    }else if(this->type == "P5"){
                        local[(dy + 1) * 3 + dx + 1].R = PNM::toIntensive(this->matrix->matrix[this->matrix->getPosition(h + dy, w + dx)]);
                        local[(dy + 1) * 3 + dx + 1].G = local[(dy + 1) * 3 + dx + 1].R;
                        local[(dy + 1) * 3 + dx + 1].B = local[(dy + 1) * 3 + dx + 1].R;
                    }

                }
            }

            COLOR softMin = PNM::minPixel({local[1], local[3], local[7], local[5], local[4]});
            COLOR softMax = PNM::maxPixel({local[1], local[3], local[7], local[5], local[4]});



            COLOR localMin = PNM::minPixel({softMin, local[0], local[2], local[6], local[8]});
            COLOR localMax = PNM::maxPixel({softMin, local[0], local[2], local[6], local[8]});

            localMin.R += softMin.R; localMin.G += softMin.G; localMin.B += softMin.B;
            localMax.R += softMax.R; localMax.G += softMax.G; localMax.B += softMax.B;


            COLOR amp;
            amp.R = sqrt(std::min(localMin.R, 2.0 - localMax.R) / localMax.R);
            amp.G = sqrt(std::min(localMin.G, 2.0 - localMax.G) / localMax.G);
            amp.B = sqrt(std::min(localMin.B, 2.0 - localMax.B) / localMax.B);


            double peak = (-1.0 / PNM::getIntensiveWithOpacity(8.0, 5.0, sharpness));
            COLOR W;
            W.R = amp.R * peak;
            W.G = amp.G * peak;
            W.B = amp.B * peak;


            double rcpWeightR = (1.0 + 4.0 * W.R);
            double rcpWeightG = (1.0 + 4.0 * W.G);
            double rcpWeightB = (1.0 + 4.0 * W.B);

            COLOR result;
            result.R = (local[1].R * W.R + local[3].R * W.R + local[4].R + local[5].R * W.R + local[7].R * W.R);
            result.G = (local[1].G * W.G + local[3].G * W.G + local[4].G + local[5].G * W.G + local[7].G * W.G);
            result.B = (local[1].B * W.B + local[3].B * W.B + local[4].B + local[5].B * W.B + local[7].B * W.B);

            result.R /= rcpWeightR;
            result.G /= rcpWeightG;
            result.B /= rcpWeightB;

//            std::cout << result.B << std::endl;
//            return;
            if(this->type == "P6") {
                new_matrix->matrix[new_matrix->getPosition(h, w) + 0] = PNM::toColor(result.R);
                new_matrix->matrix[new_matrix->getPosition(h, w) + 1] = PNM::toColor(result.G);
                new_matrix->matrix[new_matrix->getPosition(h, w) + 2] = PNM::toColor(result.B);
            }else if(this->type == "P5"){
                new_matrix->matrix[new_matrix->getPosition(h, w)] = PNM::toColor(result.R);
            }

            //return;
        }
    }

    delete this->matrix;
    this->matrix = new_matrix;
}

COLOR PNM::minPixel(COLOR_LIST list) {
    COLOR clr;
    clr = list[0];
    for(int i = 1; i < list.size(); i++){
        clr.R = std::min(clr.R, list[i].R);
        clr.G = std::min(clr.G, list[i].G);
        clr.B = std::min(clr.B, list[i].B);
    }
    return clr;
}
COLOR PNM::maxPixel(COLOR_LIST list) {
    COLOR clr;
    clr = list[0];
    for(int i = 1; i < list.size(); i++){
        clr.R = std::max(clr.R, list[i].R);
        clr.G = std::max(clr.G, list[i].G);
        clr.B = std::max(clr.B, list[i].B);
    }
    return clr;
}
double PNM::getIntensiveWithOpacity(double background, double intensive, double opacity) {
    return background * (1 - opacity) + intensive * opacity;
}

double PNM::toColor(double color) {
    return (uchar)std::min(std::max((int)(color * 255.0), 0), 255);
}

double PNM::toIntensive(uchar color) {
    return (double)std::min(std::max(((double)color / 255.0), 0.0), 1.0);
}
