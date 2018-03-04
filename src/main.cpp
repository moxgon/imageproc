#include <string>
#include <iostream>

#include "image.h"
#include "proc.h"

int main (int argc, char **argv) {
    std::string command = argv[1];
    std::string inpath = argv[2];
    std::string outpath = argv[3];

    if (command == "inv") {
        image::invert<image::gray_pix>(inpath, outpath);
    } else if (command == "heq") {
        image::hist_equalize<image::gray_pix>(inpath, outpath);
    } else if (command == "ave") {
        image::convolve<image::gray_pix>(inpath, outpath, {{1, 1, 1},{1, 1, 1},{1, 1, 1}}, 9); 
    } else if (command == "med") {
        image::median<image::gray_pix>(inpath, outpath); 
    } else if (command == "lap") {
        image::laplacian<image::gray_pix>(inpath, outpath);
    } else if (command == "hib") {
        image::highboost<image::gray_pix>(inpath, outpath);
    } else if (command == "sob") {
        image::sobel<image::gray_pix>(inpath, outpath);
    } else if (command == "rgbeq") {
        image::hist_equalize<image::rgb_pix>(inpath, outpath);
    }
}   