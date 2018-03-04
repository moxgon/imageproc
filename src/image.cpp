#include "image.h" 

namespace image {
        
    rgb_pix hsi_to_rgb(hsi_pix hsi) {
        double r,g,b;
        if ((hsi.hue >= 0) && hsi.hue < (2*pi/3)) {
            b = (1 - hsi.saturation) / 3;
            r = (1 + hsi.saturation * cos(hsi.hue) / cos(pi/3 - hsi.hue)) / 3;
            g = 1 - r - b;
        } else if ((hsi.hue >= (2*pi / 3)) && hsi.hue < (4*pi/3)) {
            hsi.hue = hsi.hue - 2*pi/3;
            r = (1 - hsi.saturation) / 3;
            g = (1 + hsi.saturation * cos(hsi.hue) / cos(pi/3 - hsi.hue)) / 3;
            b = 1 - r - g;
        } else if ((hsi.hue >= (4*pi/3)) && hsi.hue < (2*pi)) {
            hsi.hue = hsi.hue - 4*pi/3;
            g = (1 - hsi.saturation) / 3;
            b = (1 + hsi.saturation * cos(hsi.hue) / cos(pi/3 - hsi.hue)) / 3;
            r = 1 - b - g;
        }
        r *= 3*hsi.intensity;
        g *= 3*hsi.intensity;
        b *= 3*hsi.intensity;
        return {255.0*r,255.0*g,255.0*b};
    }

    hsi_pix rgb_to_hsi(rgb_pix rgb) {
        // RGB vals normalized to [0 1]
        
        double r = rgb.red/255.0;
        double g = rgb.green/255.0;
        double b = rgb.blue/255.0; 
        double intensity = (r + g + b);
        r /= intensity;
        g /= intensity;
        b /= intensity;
        intensity /= 3.0;

        double top = 0.5 * ((r-g)+(r-b));
        double bottom = sqrt((r-g)*(r-g)+(r-b)*(g-b));

        auto hue = acos(top/bottom);
        hue = b > g ? 2*pi - hue : hue;
        hue *= 180.0/pi;
        auto saturation = 1 - 3 * std::min({r, g, b});

        return {hue, saturation, intensity};
    }

/*
    template<> 
    void baseimage<png::rgb_pixel>::set_pixel_intensity(uint x, uint y, char i) {
        //auto pixel = m_image.hsi_to_rgb();
        m_image.set_pixel(x, y, i);
    }*/
}