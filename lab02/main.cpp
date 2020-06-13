#include <iostream>
#include "source/PNM.h"
#pragma pack(1)

void printError(std::string msg){
    std::cerr<< "Line drawing failed!" << std::endl;
    std::cerr << "[ERROR]: " << msg << std::endl;
}

int main(int argc, char** argv) {
    if(argc >= 9){
        PNM* image = nullptr;
        try {
            std::string inputFileName = argv[1];
            std::string outputFileName = argv[2];
            uchar brightness = (uchar) std::atoi(argv[3]);
            double thickness = std::stod(argv[4]);
            double gamma = -1;

            Point from(std::stod(argv[5]), std::stod(argv[6]));
            Point to(std::stod(argv[7]), std::stod(argv[8]));

            if (argc >= 10) {
                gamma = std::stod(argv[9]);
            }
            image = new PNM();
            image->read(inputFileName);
            image->drawLine(from, to, thickness, {brightness}, gamma);
            image->write(outputFileName);

            delete image;
            return 0;
        }catch(std::bad_alloc& error){
            printError("Memory allocation error");
        }catch(std::exception& error){
            printError(error.what());
        }
        delete image;
    }else{
        printError("Arguments are not correct");
    }
    return 1;
}