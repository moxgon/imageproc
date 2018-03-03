#include "image.h"
#include <algorithm>
#include <functional>

namespace image {  
    gs_image::gs_image(const std::string& in_path)
    : m_image(in_path) {

    }
    gs_image::~gs_image() {

    }

    void gs_image::set_pixel(uint x, uint y, char i) {
        m_image.set_pixel(x, y, i);
    }

    uint gs_image::get_pixel(uint x, uint y) const {
        return m_image.get_pixel(x, y);
    }

    void gs_image::save(const std::string& out_path) {
        m_image.write(out_path);
    }

    int gs_image::get_height() const {
        return m_image.get_height();
    }

    int gs_image::get_width() const {
        return m_image.get_width();
    }

    png::image<png::gray_pixel>::pixbuf& gs_image::get_buffer() {
        return m_image.get_pixbuf();
    }

}