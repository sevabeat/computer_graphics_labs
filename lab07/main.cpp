#include <iostream>
#include "source/PNM.h"
#pragma pack(1)

void printError(std::string msg){
    std::cerr << "Failed!" << std::endl;
    std::cerr << "[ERROR]: " << msg << std::endl;
}

int main(int argc, char** argv) {
    auto image = new PNM();

    if(argc >= 4){
        std::string inputFileName = argv[1];
        std::string outputFileName = argv[2];
        double sharp = std::stod(argv[3]);

        PNM* image = nullptr;
        try {
            image = new PNM();
            image->read(inputFileName);
            if(sharp >= 0.0 && sharp <= 1.0){
                image->ContrastAdaptiveSharpening(sharp);
                image->write(outputFileName);
                delete image;
                return 0;
            }else{
                printError("Arguments are not correct");
            }
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