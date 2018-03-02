#include <string>
#include <iostream>

#include "image.h"

int main (int argc, char **argv) {
    std::string inpath = argv[1];
    std::string outpath = argv[2];
    image::invert(inpath, outpath);
}   