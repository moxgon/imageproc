#pragma once

#include <string>
#include <png++/png.hpp>

namespace image {
    class gs_image {
        public:
        gs_image(std::string in_path);
        ~gs_image();

        void set_pixel(int x, int y, char i);
        int get_pixel(int x, int y) const;

        void save(std::string out_path);
        private:
        png::image<png::gray_pixel> m_image;
    };
}