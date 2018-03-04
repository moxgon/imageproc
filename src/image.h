#pragma once

#include <string>
#include <png++/png.hpp>
#include "proc.h"

namespace image {
    using rgb_pix = png::rgb_pixel;
    using gray_pix = png::gray_pixel;

    struct hsi_pix {
        double hue;
        double saturation;
        double intensity;
    };

    hsi_pix rgb_to_hsi(rgb_pix rgb);
    rgb_pix hsi_to_rgb(hsi_pix hsi);

    template<class T>
    class baseimage {
        public:

        baseimage(const std::string& in_path)
        : m_image(in_path) {}
        
        ~baseimage() {}

        void set_pixel(uint x, uint y, T p) {
            m_image.set_pixel(x, y, p);
        }

        T get_pixel(uint x, uint y) const {
            return m_image.get_pixel(x,y);
        }

        void save(const std::string& out_path) {
            m_image.write(out_path);
        }

        int get_height() const {
            return m_image.get_height();
        }

        int get_width() const {
            return m_image.get_width();
        }

        typename png::image<T>::pixbuf& get_buffer() {
            return m_image.get_pixbuf();
        }

        private:
        png::image<T> m_image;
    };


    using gs_image = baseimage<png::gray_pixel>;
    using rgb_image = baseimage<rgb_pix>;
}