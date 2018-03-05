#include "image.h" 

namespace image {
        
    rgb_pix hsi_to_rgb(hsi_pix hsi) {
        double r,g,b;
        auto h = hsi.hue * pi/180.0;
        if (hsi.saturation > 1) hsi.saturation = 1;
        if (hsi.intensity > 1) hsi.intensity = 1;
        if (hsi.saturation == 0) {
            r = g = b = hsi.intensity;
        } else {
            if ((h >= 0) && h < (2*pi/3)) {
                b = (1 - hsi.saturation) / 3;
                r = (1 + hsi.saturation * cos(h) / cos(pi/3 - h)) / 3;
                g = 1 - r - b;
            } else if ((h >= (2*pi / 3)) && h < (4*pi/3)) {
                h = h - 2*pi/3;
                r = (1 - hsi.saturation) / 3;
                g = (1 + hsi.saturation * cos(h) / cos(pi/3 - h)) / 3;
                b = 1 - r - g;
            } else if ((h >= (4*pi/3)) && h< (2*pi)) {
                h = h - 4*pi/3;
                g = (1 - hsi.saturation) / 3;
                b = (1 + hsi.saturation * cos(h) / cos(pi/3 - h)) / 3;
                r = 1 - b - g;
            }  
            if (r < 0) r = 0;
            if (g < 0) g = 0;
            if (b < 0) b = 0;

            r *= 3*hsi.intensity;
            g *= 3*hsi.intensity;
            b *= 3*hsi.intensity;

            if (r > 1) r = 1;
            if (g > 1) g = 1;
            if (b > 1) b = 1;
        }
        return {255.0*r,255.0*g,255.0*b};
    }

    hsi_pix rgb_to_hsi(rgb_pix rgb) {
        // RGB vals normalized to [0 1]
        double hue = 0;
        double saturation = 0; 

        double r = rgb.red/255.0;
        double g = rgb.green/255.0;
        double b = rgb.blue/255.0; 
        double intensity = (r + g + b);

        if (intensity != 0) {
            r /= intensity;
            g /= intensity;
            b /= intensity;
        }
        intensity /= 3.0;

        if (rgb.red == rgb.green && rgb.green == rgb.blue) {
            hue = 0;
            saturation = 0;
        } else {
            double w = 0.5 * ((r-g)+(r-b)) / sqrt((r-g)*(r-g)+(r-b)*(g-b));
            if (w > 1) w = 1;
            if (w < -1) w = -1;
            hue = acos(w);
            if (hue < 0) std::cout << "H<0: " << hue << std::endl;
            hue = b > g ? 2*pi - hue : hue;
            hue *= 180.0/pi;
            saturation = 1 - 3 * std::min({r, g, b});
        }
        return {hue, saturation, intensity};
    }

/*
    template<> 
    void baseimage<png::rgb_pixel>::set_pixel_intensity(uint x, uint y, char i) {
        //auto pixel = m_image.hsi_to_rgb();
        m_image.set_pixel(x, y, i);
    }*/
}