#pragma once

#include <string>
#include <png++/png.hpp>
#include "proc.h"

namespace image {
    using gray_pix = png::gray_pixel;
    using rgb_pix = png::rgb_pixel;

    struct hsi {
        double hue;
        double saturation;
        double intensity;
    };

    hsi rgb_to_hsi(rgb_pix rgb);

    template<class T>
    class baseimage {
        public:

        baseimage(const std::string& in_path)
        : m_image(in_path) {}
        
        ~baseimage() {}

        void set_pixel(uint x, uint y, T p) {
            m_image.set_pixel(x, y, p);
        }

        uint get_pixel_intensity(uint x, uint y) const {
            return m_image.get_pixel(x, y);
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

    template<>
    uint baseimage<png::rgb_pixel>::get_pixel_intensity(uint x, uint y) const {
            return rgb_to_hsi(m_image.get_pixel(x,y)).intensity;
    }

    hsi rgb_to_hsi(rgb_pix rgb) {
        // RGB vals normalized to [0 1]
        double r = 255.0/rgb.red;
        double g = 255.0/rgb.green; 
        double b = 255.0/rgb.blue;

        double top = 0.5 * ((r-g)+(r-b));
        double bottom = sqrt(((r-g)*(r-g)+(r-b)*(g-b)));

        auto hue = acos(top/bottom) * 180.0 / pi;
        hue = b > g ? 360 - hue : hue;
        hue /= 360;

        auto intensity = (r + g + b) / 3;

        auto saturation = 1 - (3 * std::min({r, g, b})/intensity);

        return {hue, saturation, intensity};
    }

    using gs_image = baseimage<png::gray_pixel>;
    using rgb_image = baseimage<rgb_pix>;
}