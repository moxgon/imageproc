#pragma once
#include <string>
#include <vector>

namespace image {
    void invert(const std::string& in_path, const std::string& out_path);

    void hist_equalize(const std::string& in_path, const std::string& out_path);

    void convolve(const std::string& in_path, const std::string& out_path, const std::vector<std::vector<int>>& kernel, uint divisor = 1);

    void median(const std::string& in_path, const std::string& out_path);

    void laplacian(const std::string& in_path, const std::string& out_path);

    void highboost(const std::string& in_path, const std::string& out_path);

    void sobel(const std::string& in_path, const std::string& out_path);
}