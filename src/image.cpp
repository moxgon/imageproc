#include "image.h" 

namespace image {
        
    rgb_pix hsi_to_rgb(hsi_pix hsi) {

        return {255,255,255};
    }

    hsi_pix rgb_to_hsi(rgb_pix rgb) {
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

/*
    template<> 
    void baseimage<png::rgb_pixel>::set_pixel_intensity(uint x, uint y, char i) {
        //auto pixel = m_image.hsi_to_rgb();
        m_image.set_pixel(x, y, i);
    }*/
}