#include <string>
#include <iostream>

#include "image.h"

int main (int argc, char **argv) {
    std::string command = argv[1];
    std::string inpath = argv[2];
    std::string outpath = argv[3];

    if (command == "invert") {
        image::invert(inpath, outpath);
    } else if (command == "hist_equalize") {
        image::hist_equalize(inpath, outpath);
    } else if (command == "convolve") {
        image::convolve(inpath, outpath, {{1, 2, 1},{0, 0, 0},{-1, -2, -1}}); // Sobel ya
    }
}   