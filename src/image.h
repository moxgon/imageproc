#pragma once

#include <string>
#include <png++/png.hpp>

namespace image {
    class gs_image {
        public:
        gs_image(const std::string& in_path);
        ~gs_image();

        uint get_pixel(uint x, uint y) const;
        void set_pixel(uint x, uint y, char i);
        
        int get_height() const;
        int get_width() const;

        png::image<png::gray_pixel>::pixbuf& get_buffer();
        
        void save(const std::string& out_path);

        private:
        png::image<png::gray_pixel> m_image;
    };

    void invert(const std::string& in_path, const std::string& out_path);

    void hist_equalize(const std::string& in_path, const std::string& out_path);

    void convolve(const std::string& in_path, const std::string& out_path, const std::vector<std::vector<int>>& mask);
}