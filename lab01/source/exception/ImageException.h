#ifndef PROG_IMAGEEXCEPTION_H
#define PROG_IMAGEEXCEPTION_H

#include <exception>
#include <string>
#include <utility>

class ImageException : public std::exception{
    private:
    std::string msg;
    public:
        ImageException(std::string msg): msg(std::move(msg)) {}
        virtual const char* what(){
            return this->msg.c_str();
        }
};


#endif //PROG_IMAGEEXCEPTION_H
