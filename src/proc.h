#pragma once

#include <algorithm>
#include <functional>
#include <cmath>

#include "image.h"

namespace image {
    template<class T> class baseimage;
}

namespace { 

    const double pi = 3.14159265;

    // Assumes square neighborhood
    template<class T>
    std::vector<T> get_neighbors(const image::baseimage<T>& img, uint x, uint y, uint size) {
        std::vector<T> neighbors;
        for (uint row = 0; row < size; row++) {
            for (uint col = 0; col < size; col++) {
                neighbors.push_back(img.get_pixel_intensity(x + col - 1, y + row - 1));
            }
        }
        return neighbors;
    }

    template<class T>
    int apply_kernel(const image::baseimage<T>& img, const std::vector<std::vector<int>>& kernel, uint x, uint y) {
        int sum = 0;

        for (uint row = 0; row < kernel.size(); row++) {
            for (uint col = 0; col < kernel[row].size(); col++) {
                sum += kernel[row][col] * img.get_pixel_intensity(x + col - 1, y + row - 1);
            }
        }

        return sum;
    }

    template<class T>
    void process_no_edges(const image::baseimage<T>& img, std::function<void(uint,uint,T)> func) {
        for (int y = 1; y < img.get_height() - 1; y++) {
            for (int x = 1; x < img.get_width() - 1; x++) {
               func(x, y, img.get_pixel_intensity(x, y)); // Apply function 
            }
        }
    }

    template<class T>
    void process(const image::baseimage<T>& img, std::function<void(uint,uint,T)> func) {
        // For each pixel
        for (int y = 0; y < img.get_height(); y++) {
            for (int x = 0; x < img.get_width(); x++) {
               func(x, y, img.get_pixel_intensity(x, y)); // Apply function 
            }
        }
    }

    template<class T>
    std::vector<uint> generate_histogram(const std::string& in_path) {
        image::baseimage<T> img(in_path);
        std::vector<uint> hist (256, 0);

        process<T>(img, [&hist] (uint x, uint y, uint p) {
            hist[p]++;
        });

        return hist;
    }

    template<class T>
    void map_by(const std::string& in_path, const std::string& out_path, const std::vector<uint>& mapping) {
        image::baseimage<T> img(in_path);

        process<T>(img, [&img,&mapping] (uint x, uint y, uint p) {
            img.set_pixel(x, y, mapping[p]);
        });

        img.save(out_path);
    }
}

namespace image {

        template<class T>
        void invert(const std::string& in_path, const std::string& out_path) {
        baseimage<T> img(in_path);

        process<T>(img, [&img] (uint x, uint y, uint p) {
            img.set_pixel(x, y, 255 - p);
        });

        img.save(out_path);
    }

    template<class T>
    void hist_equalize(const std::string& in_path, const std::string& out_path) {
        baseimage<T> img(in_path);

        auto h = generate_histogram<T>(in_path);

        std::vector<uint> equalized;
        
        uint sum = 0;
        for (const auto i: h) {
            sum += i;
            auto pixel = (sum * 255) / (img.get_width() * img.get_height());
            equalized.push_back(pixel);
        }

        map_by<T>(in_path, out_path, equalized);
    }

    template<class T>
    void convolve(const std::string& in_path, const std::string& out_path, const std::vector<std::vector<int>>& kernel, uint divisor) {
        baseimage<T> img(in_path);
        baseimage<T> out(in_path);

        process_no_edges<T>(img, [&img, &out, &kernel, &divisor] (uint x, uint y, uint p) {
            auto sum = apply_kernel(img, kernel, x, y);
            out.set_pixel(x, y, abs(sum)/divisor);
        });
        
        out.save(out_path);
    }

    template<class T>
    void median(const std::string& in_path, const std::string& out_path) {
        baseimage<T> img(in_path);
        baseimage<T> out(in_path);

        process_no_edges<T>(img, [&img, &out] (uint x, uint y, uint p) {
            auto v = get_neighbors(img, x, y, 3);
            size_t n = v.size() / 2;
            nth_element(v.begin(), v.begin()+n, v.end());
            out.set_pixel(x, y, v[n]);
        });

        out.save(out_path);
    }

    template<class T>
    void laplacian(const std::string& in_path, const std::string& out_path) {
        baseimage<T> img(in_path);
        baseimage<T> out(in_path);

        process_no_edges<T>(img, [&img, &out] (uint x, uint y, uint p) {
            //auto sum = apply_kernel(img, {{-1, -1, -1},{-1, 8, -1},{-1, -1, -1}}, x, y);
            //out.set_pixel(x, y, img.get_pixel(x, y) + abs(sum));
        });
        
        out.save(out_path);
    }

    template<class T>
    void highboost(const std::string& in_path, const std::string& out_path) {
        baseimage<T> img(in_path);
        baseimage<T> out(in_path);

        process_no_edges<T>(img, [&img, &out] (uint x, uint y, uint p) {
            //auto sum = apply_kernel(img, {{-1, -1, -1},{-1, 4 + img.get_pixel(x, y), -1},{-1, -1, -1}}, x, y);
            //out.set_pixel(x, y, abs(sum));
        });
        
        out.save(out_path);
    }

    template<class T>
    void sobel(const std::string& in_path, const std::string& out_path) {
        baseimage<T> img(in_path);
        baseimage<T> out(in_path);

        process_no_edges<T>(img, [&img, &out] (uint x, uint y, uint p) { 
            auto sum_x = apply_kernel(img, {{-1, 0, 1},{-2, 0, 2},{-1, 0, 1}}, x, y)/4;
            auto sum_y = apply_kernel(img, {{-1, -2, -1},{0, 0, 0},{1, 2, 1}}, x, y)/4; 
            out.set_pixel(x, y, abs(sum_x) + abs(sum_y));
        });

        out.save(out_path);
    }
}