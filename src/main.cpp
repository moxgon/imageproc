#include <string>
#include <iostream>

#include "image.h"
#include "proc.h"

int main (int argc, char **argv) {
    std::string command = argv[1];
    std::string inpath = argv[2];
    std::string outpath = argv[3];

    if (command == "inv") {
        image::invert(inpath, outpath);
    } else if (command == "heq") {
        image::hist_equalize(inpath, outpath);
    } else if (command == "ave") {
        image::convolve(inpath, outpath, {{1, 1, 1},{1, 1, 1},{1, 1, 1}}, 9); 
    } else if (command == "med") {
        image::median(inpath, outpath); 
    } else if (command == "lap") {
        image::laplacian(inpath, outpath);
    } else if (command == "hib") {
        image::highboost(inpath, outpath);
    } else if (command == "sob") {
        image::sobel(inpath, outpath);
    }
}   