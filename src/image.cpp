#include "image.h"
#include <algorithm>

namespace {

    std::vector<int> generate_histogram(const std::string& in_path) {
        image::gs_image img(in_path);
        std::vector<int> hist (256, 0);

        for (int y = 0; y < img.get_height(); y++) {
            for (int x = 0; x < img.get_width(); x++) {
                auto intensity = img.get_pixel(x, y);
                hist[intensity]++;
            }
        }
        return hist;
    }

    void map_by(const std::string& in_path, const std::string& out_path, const std::vector<int>& mapping) {
        image::gs_image img(in_path);

        for (int y = 0; y < img.get_height(); y++) {
            for (int x = 0; x < img.get_width(); x++) {
                auto intensity = img.get_pixel(x, y);
                img.set_pixel(x, y, mapping[intensity]);
            }
        }

        img.save(out_path);
    }
}


namespace image {  
    gs_image::gs_image(const std::string& in_path)
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

        for (int y = 0; y < img.get_height(); y++) {
            for (int x = 0; x < img.get_width(); x++) {
                auto intensity = img.get_pixel(x, y);
                img.set_pixel(x, y, 255 - intensity);
            }
        }

        img.save(out_path);
    }

    void hist_equalize(const std::string& in_path, const std::string& out_path) {
        gs_image img(in_path);
        auto h = generate_histogram(in_path);
        std::vector<int> equalized;
        
        int sum = 0;
        for (const auto i: h) {
            sum += i;
            int intensity = (sum*255)/(img.get_width()*img.get_height());
            equalized.push_back(intensity);
        }

        map_by(in_path, out_path, equalized);
    }

    void convolve(const std::string& in_path, const std::string& out_path, const std::vector<std::vector<int>>& mask) {
        gs_image img(in_path);

        for (int y = 1; y < img.get_height() - 1; y++) {
            int x; 
            for (x = 1; x < img.get_width() - 1; x++) {
                int sum = 0;
                for (int row = 0; row < mask.size(); row++) {
                    for (int col = 0; col < mask[row].size(); col++) {
                        sum += mask[row][col] * img.get_pixel(x + col - 1, y + row - 1);
                    }
                }
                std::cout << abs(sum) << std::endl;
                img.set_pixel(x, y, abs(sum)/2);
            }
        }

        img.save(out_path);
    }

}