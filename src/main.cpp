#include <string>
#include <iostream>

#include "image.h"

int main (int argc, char **argv) {
    std::string imgpath = argv[1];
    image::gs_image img(imgpath);

    std::cout << img.get_pixel(0, 0) << std::endl;
}   