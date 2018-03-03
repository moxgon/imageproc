#include "image.h"
#include <algorithm>
#include <functional>

namespace {
    int apply_kernel(const image::gs_image& img, const std::vector<std::vector<int>>& kernel, uint x, uint y) {
        float sum = 0;
        for (uint row = 0; row < kernel.size(); row++) {
            for (uint col = 0; col < kernel[row].size(); col++) {
                sum += kernel[row][col] * img.get_pixel(x + col - 1, y + row - 1) / 4;
            }
        }
        return sum;
    }

    void process(const image::gs_image& img, std::function<void(uint,uint,uint)> func, uint start_x = 0, uint start_y = 0) {
        // For each pixel
        for (int y = start_y; y < img.get_height(); y++) {
            for (int x = start_x; x < img.get_width(); x++) {
               func(x, y, img.get_pixel(x, y)); // Apply function 
            }
        }
    }
    std::vector<uint> generate_histogram(const std::string& in_path) {
        image::gs_image img(in_path);
        std::vector<uint> hist (256, 0);

        /*for (int y = 0; y < img.get_height(); y++) {
            for (int x = 0; x < img.get_width(); x++) {
                auto intensity = img.get_pixel(x, y);
                hist[intensity]++;
            }
        }*/

        process(img, [&hist] (uint x, uint y, uint p) {
            hist[p]++;
        });

        return hist;
    }

    void map_by(const std::string& in_path, const std::string& out_path, const std::vector<uint>& mapping) {
        image::gs_image img(in_path);

        process(img, [&img,&mapping] (uint x, uint y, uint p) {
            img.set_pixel(x, y, mapping[p]);
        });

        /*
        for (int y = 0; y < img.get_height(); y++) {
            for (int x = 0; x < img.get_width(); x++) {
                auto intensity = img.get_pixel(x, y);
                img.set_pixel(x, y, mapping[intensity]);
            }
        }
        */

        img.save(out_path);
    }
}


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

    void invert(const std::string& in_path, const std::string& out_path) {
        gs_image img(in_path);

        process(img, [&img] (uint x, uint y, uint p) {
            img.set_pixel(x, y, 255 - p);
        });
        /*
        for (int y = 0; y < img.get_height(); y++) {
            for (int x = 0; x < img.get_width(); x++) {
                auto intensity = img.get_pixel(x, y);
                img.set_pixel(x, y, 255 - intensity);
            }
        }*/

        img.save(out_path);
    }

    void hist_equalize(const std::string& in_path, const std::string& out_path) {
        gs_image img(in_path);

        auto h = generate_histogram(in_path);

        std::vector<uint> equalized;
        
        uint sum = 0;
        for (const auto i: h) {
            sum += i;
            uint intensity = (sum * 255) / (img.get_width() * img.get_height());
            equalized.push_back(intensity);
        }

        map_by(in_path, out_path, equalized);
    }

    void convolve(const std::string& in_path, const std::string& out_path, const std::vector<std::vector<int>>& mask) {
        gs_image img(in_path);
        gs_image out(in_path);

        process(img,
            [&img, &out, &mask] (uint x, uint y, uint p) {
                auto sum = apply_kernel(img, mask, x, y);
                out.set_pixel(x, y, abs(sum));
            }, 1, 1);
        
        out.save(out_path);
    }

}