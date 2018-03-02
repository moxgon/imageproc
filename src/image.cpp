#include "image.h"

namespace image {  
    gs_image::gs_image(std::string in_path)
    : m_image(in_path) {

    }
    gs_image::~gs_image() {

    }

    void gs_image::set_pixel(int x, int y, char i) {
        m_image.set_pixel(x, y, i);
    }

    int gs_image::get_pixel(int x, int y) const {
        return m_image.get_pixel(x, y);
    }

    void gs_image::save(std::string out_path) {
        m_image.write(out_path);
    }
}