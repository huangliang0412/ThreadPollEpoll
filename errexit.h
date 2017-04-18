//#ifndef ERREXIT_H
//#define ERREXIT_H
#pragma once
#include <iostream>
#include <errno.h>
#include <string>
#include <string.h>
inline int errExit(std::string errostring) {
    std::cout << errostring;
    std::cout << strerror(errno);
    return -1;
}

//#endif // ERREXIT_H
