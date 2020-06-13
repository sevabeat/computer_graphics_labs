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
void PNM::showMetaData(){
    std::cout << "width: " << this->matrix->getWidth() << std::endl;
    std::cout << "height: " << this->matrix->getHeight() << std::endl;
    std::cout << "depth: " << this->matrix->getDepth() << std::endl;
    std::cout << "comments:" << std::endl;
    for(int i = 0; i < this->comments.size(); i++){
        std::cout << this->comments[i] << std::endl;
    }
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

void PNM::drawLine(Point from, Point to, double thickness,  PIXEL_COLOR color, double gamma) {
    //Нахожу вектор толщины через скларяное произведение
    // x1 * x2 + y1 * y2 = 0 - векторы перпендикулярны
    // x1 = -(y1 * y2) / x2
    // y1 = -(x1 * x2) / y2

    Point line_vector(to.x - from.x, to.y - from.y);
    Point thickness_vector(0,0);

    if(line_vector.x != 0){
        thickness_vector = {-1.0 * line_vector.y / line_vector.x, 1.0};
    }else if(line_vector.y != 0){
        thickness_vector = {1.0, -1.0 * line_vector.x / line_vector.y};
    }
    thickness_vector *= ((thickness / 2) / thickness_vector.length());

    Point p1 = from - thickness_vector;
    Point p2 = to - thickness_vector;
    Point p3 = to + thickness_vector;
    Point p4 = from + thickness_vector;


    auto line = new Rectangle(p1, p2, p3, p4);

    for(double x = std::max(line->getMinX() - 2, 0.0); x < std::min(line->getMaxX() + 3, (double)this->matrix->getWidth()); x++){
        for(double y = std::max(line->getMinY() - 2, 0.0); y < std::min(line->getMaxY() + 3, (double)this->matrix->getHeight()); y++){
            double area = line->commonArea({x, y});
            if(area > 0){
                this->matrix->drawPoint({x, y}, color, area, gamma);
            }
        }
    }
    delete line;
}

PNM::~PNM(){
    delete this->matrix;
}