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

    std::vector<int> generate_histogram(const std::string& in_path) {
        gs_image img(in_path);
        std::vector<int> hist (255, 0);

        for (int y = 0; y < img.get_height(); y++) {
            for (int x = 0; x < img.get_width(); x++) {
                auto intensity = img.get_pixel(x, y);
                hist.at(intensity)++;
            }
        }

        return hist;
    }

    void map_by(const std::string& in_path, const std::string& out_path, const std::vector<int>& mapping) {
        gs_image img(in_path);

        for (int y = 0; y < img.get_height(); y++) {
            for (int x = 0; x < img.get_width(); x++) {
                auto intensity = img.get_pixel(x, y);
                img.set_pixel(x, y, mapping.at(intensity));
            }
        }

        img.save(out_path);
    }

    
    void hist_equalize(const std::string& in_path, const std::string& out_path) {
        auto h = generate_histogram(in_path);
        std::vector<int> equalized (255, 0);

        int sum = 0;
        for (const auto& i: h) {
            sum += i;
            equalized.push_back(sum);
        }
        map_by(in_path, out_path, equalized);
    }


}