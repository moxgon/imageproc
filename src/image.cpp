#include "image.h" 

namespace image {
        
    rgb_pix hsi_to_rgb(hsi_pix hsi) {
        double r,g,b;
        auto h = hsi.hue * pi/180.0; // To radians

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

            // Dumb way to write less code, but I like it
            for (auto* c : {&r, &g, &b}) {
                if (*c < 0) *c = 0;
                *c *= 3*hsi.intensity;
                if (*c > 1) *c = 1;
            }
        }
        return {255.0*r,255.0*g,255.0*b}; // Normalize back to [0 255]
    }

    hsi_pix rgb_to_hsi(rgb_pix rgb) {
        double hue = 0;
        double saturation = 0; 

        // Normalize RGB values to [0 1]
        double r = rgb.red/255.0;
        double g = rgb.green/255.0;
        double b = rgb.blue/255.0; 
        double intensity = (r + g + b);

        if (intensity != 0) { // Avoid division by zero
            r /= intensity;
            g /= intensity;
            b /= intensity;
        }

        intensity /= 3.0;

        if (rgb.red == rgb.green && rgb.green == rgb.blue) {
            hue = 0;
            saturation = 0;
        } else {
            double w = 0.5 * ((r-g) + (r-b)) / sqrt((r-g) * (r-g) + (r-b) * (g-b));

            if (w > 1) w = 1;
            if (w < -1) w = -1;

            hue = acos(w);
            hue = b > g ? 2*pi - hue : hue;
            hue *= 180.0/pi; // To degrees

            saturation = 1 - 3 * std::min({r, g, b});
        }
        return {hue, saturation, intensity};
    }
}