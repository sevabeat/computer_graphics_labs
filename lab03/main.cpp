#include <iostream>
#include "source/PNM.h"
#include "source/Dither.h"
#pragma pack(1)



void printError(std::string msg){
    std::cerr<< "Line drawing failed!" << std::endl;
    std::cerr << "[ERROR]: " << msg << std::endl;
}

int main(int argc, char** argv) {
    if(argc >= 7){
        PNM* image = nullptr;
        try {
            std::string inputFileName = argv[1];
            std::string outputFileName = argv[2];

            bool gradient = std::atoi(argv[3]) == 1;
            int ditherType = std::atoi(argv[4]);
            int bits =  std::atoi(argv[5]);
            double gamma = std::stod(argv[6]);
            if(gamma == 0) gamma = -1;

            image = new PNM();
            image->read(inputFileName);
            if(gradient){
                image->drawGradient(gamma);
            }else{
                image->gammaCorrection(0, gamma);
            }
            image->dithering(ditherType, bits);
            image->gammaCorrection(1, gamma);
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