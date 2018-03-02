#pragma once

#include <string>
#include <png++/png.hpp>

namespace image {
    class gs_image {
        public:
        gs_image(std::string in_path);
        ~gs_image();

        int get_pixel(int x, int y) const;
        void set_pixel(int x, int y, char i);
        
        int get_height() const;
        int get_width() const;

        png::image<png::gray_pixel>::pixbuf& get_buffer();
        
        void save(std::string out_path);
        private:
        png::image<png::gray_pixel> m_image;
    };

    void invert(const std::string& in_path, const std::string& out_path);

    void hist_equalize(const std::string& in_path, const std::string& out_path);
}