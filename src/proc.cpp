#include <algorithm>
#include <functional>
#include "image.h"

namespace {   
    // Assumes square neighborhood
    std::vector<uint> get_neighbors(const image::gs_image& img, uint x, uint y, uint size) {
        std::vector<uint> neighbors;
        for (uint row = 0; row < size; row++) {
            for (uint col = 0; col < size; col++) {
                neighbors.push_back(img.get_pixel(x + col - 1, y + row - 1));
            }
        }
        return neighbors;
    }

    int apply_kernel(const image::gs_image& img, const std::vector<std::vector<int>>& kernel, uint x, uint y) {
        int sum = 0;

        for (uint row = 0; row < kernel.size(); row++) {
            for (uint col = 0; col < kernel[row].size(); col++) {
                sum += kernel[row][col] * img.get_pixel(x + col - 1, y + row - 1);
            }
        }

        return sum;
    }

    void process_no_edges(const image::gs_image& img, std::function<void(uint,uint,uint)> func) {
        for (int y = 1; y < img.get_height() - 1; y++) {
            for (int x = 1; x < img.get_width() - 1; x++) {
               func(x, y, img.get_pixel(x, y)); // Apply function 
            }
        }
    }

    void process(const image::gs_image& img, std::function<void(uint,uint,uint)> func) {
        // For each pixel
        for (int y = 0; y < img.get_height(); y++) {
            for (int x = 0; x < img.get_width(); x++) {
               func(x, y, img.get_pixel(x, y)); // Apply function 
            }
        }
    }

    std::vector<uint> generate_histogram(const std::string& in_path) {
        image::gs_image img(in_path);
        std::vector<uint> hist (256, 0);

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

        img.save(out_path);
    }
}

namespace image {
        void invert(const std::string& in_path, const std::string& out_path) {
        gs_image img(in_path);

        process(img, [&img] (uint x, uint y, uint p) {
            img.set_pixel(x, y, 255 - p);
        });

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

    void convolve(const std::string& in_path, const std::string& out_path, const std::vector<std::vector<int>>& kernel, uint divisor) {
        gs_image img(in_path);
        gs_image out(in_path);

        process_no_edges(img, [&img, &out, &kernel, &divisor] (uint x, uint y, uint p) {
            auto sum = apply_kernel(img, kernel, x, y);
            out.set_pixel(x, y, abs(sum)/divisor);
        });
        
        out.save(out_path);
    }

    void median(const std::string& in_path, const std::string& out_path) {
        gs_image img(in_path);
        gs_image out(in_path);

        process_no_edges(img, [&img, &out] (uint x, uint y, uint p) {
            auto v = get_neighbors(img, x, y, 3);
            size_t n = v.size() / 2;
            nth_element(v.begin(), v.begin()+n, v.end());
            out.set_pixel(x, y, v[n]);
        });

        out.save(out_path);
    }

    void laplacian(const std::string& in_path, const std::string& out_path) {
        gs_image img(in_path);
        gs_image out(in_path);

        process_no_edges(img, [&img, &out] (uint x, uint y, uint p) {
            //auto sum = apply_kernel(img, {{-1, -1, -1},{-1, 8, -1},{-1, -1, -1}}, x, y);
            //out.set_pixel(x, y, img.get_pixel(x, y) + abs(sum));
        });
        
        out.save(out_path);
    }

    void highboost(const std::string& in_path, const std::string& out_path) {
        gs_image img(in_path);
        gs_image out(in_path);

        process_no_edges(img, [&img, &out] (uint x, uint y, uint p) {
            //auto sum = apply_kernel(img, {{-1, -1, -1},{-1, 4 + img.get_pixel(x, y), -1},{-1, -1, -1}}, x, y);
            //out.set_pixel(x, y, abs(sum));
        });
        
        out.save(out_path);
    }

    void sobel(const std::string& in_path, const std::string& out_path) {
        gs_image img(in_path);
        gs_image out(in_path);

        process_no_edges(img, [&img, &out] (uint x, uint y, uint p) { 
            auto sum_x = apply_kernel(img, {{-1, 0, 1},{-2, 0, 2},{-1, 0, 1}}, x, y)/4;
            auto sum_y = apply_kernel(img, {{-1, -2, -1},{0, 0, 0},{1, 2, 1}}, x, y)/4; 
            out.set_pixel(x, y, abs(sum_x) + abs(sum_y));
        });

        out.save(out_path);
    }
}