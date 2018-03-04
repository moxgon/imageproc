#include "proc.h"

namespace image {
    template<>
    void map_by<rgb_pix>(const std::string& in_path, const std::string& out_path, const std::vector<uint>& mapping) {
        image::baseimage<image::rgb_pix> img(in_path);

        process<rgb_pix>(img, [&img,&mapping] (uint x, uint y, image::rgb_pix p) {
            auto hsi = image::rgb_to_hsi(p);
            hsi.intensity = mapping[hsi.intensity*255.0]/255.0;
            img.set_pixel(x, y, hsi_to_rgb(hsi));
        });

        img.save(out_path);
    }

    template<>
    std::vector<uint> generate_histogram<rgb_pix>(const std::string& in_path) {
        image::baseimage<rgb_pix> img(in_path);
        std::vector<uint> hist (256, 0);

        process<rgb_pix>(img, [&img, &hist] (uint x, uint y, rgb_pix p) {
            auto hsi = image::rgb_to_hsi(p);
            hist[hsi.intensity*255.0]++;
        });

        return hist;
    }
}