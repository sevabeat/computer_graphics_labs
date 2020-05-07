#include <iostream>
#include "source/PNM.h"
#pragma pack(1)

void printError(std::string msg){
    std::cerr<< "Transformation failed!" << std::endl;
    std::cerr << "[ERROR]: " << msg << std::endl;
}

int main(int argc, char** argv) {
    if(argc >= 4){
        std::string inputFileName = argv[1];
        std::string outputFileName = argv[2];
        std::string transformation = argv[3];

        PNM* image = nullptr;
        try {
            image = new PNM();
            image->read(inputFileName);
            if (transformation == "0") {
                image->inverseColors();
            } else if (transformation == "1") {
                image->horizontalReflection();
            } else if (transformation == "2") {
                image->verticalReflection();
            } else if (transformation == "3") {
                image->rotateRight90();
            } else if (transformation == "4") {
                image->rotateLeft90();
            } else {
                printError("Arguments are not correct");
                delete image;
                return 0;
            }
            image->write(outputFileName);
            std::cout << "Success!" << std::endl;
            delete image;
        }catch(std::bad_alloc& error){
            printError("Memory allocation error");
            delete image;
        }catch(std::exception& error){
            printError(error.what());
            delete image;
        }
    }else{
        printError("Arguments are not correct");
    }
    return 0;
}