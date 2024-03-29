#include <valarray>
#include "correlation.h"

#include <map>

double BMP::calculate_mathematical_expectation(char component) const {
    if (_info_header.bi_width == 0 || _info_header.bi_height == 0) {
        throw std::runtime_error("Image dimensions are invalid.");
    }

    double sum_intensity = 0.0;
    int component_index = get_component_index(component);

    for (int i = 0; i < _info_header.bi_height; ++i) {
        for (int j = 0; j < _info_header.bi_width; ++j) {
            sum_intensity += _data[(i * _info_header.bi_width + j) * 3 + component_index];
        }
    }

    double mean_intensity = sum_intensity / (_info_header.bi_width * _info_header.bi_height);

    return mean_intensity;
}

double BMP::calculate_the_standard_deviation_estimate(char component) const {
    if (_info_header.bi_width == 0 || _info_header.bi_height == 0) {
        throw std::runtime_error("Image dimensions are invalid.");
    }

    double mean_intensity = calculate_mathematical_expectation(component);
    int component_index = get_component_index(component);

    double sum_squared_deviations = 0.0;

    for (int i = 0; i < _info_header.bi_height; ++i) {
        for (int j = 0; j < _info_header.bi_width; ++j) {
            double pixel_intensity = _data[(i * _info_header.bi_width + j) * 3 + component_index];
            double deviation = pixel_intensity - mean_intensity;
            sum_squared_deviations += deviation * deviation;
        }
    }

    int W = _info_header.bi_width;
    int H = _info_header.bi_height;
    double normalization_factor = 1.0 / (W * H - 1);

    return sqrt(normalization_factor * sum_squared_deviations);
}

double BMP::calculate_rgb_correlation(char component1, char component2) {
    double mean_component1 = calculate_mathematical_expectation(component1);
    double mean_component2 = calculate_mathematical_expectation(component2);

    int component1_index = get_component_index(component1);
    int component2_index = get_component_index(component2);

    double std_deviation_component1 = calculate_the_standard_deviation_estimate(component1);
    double std_deviation_component2 = calculate_the_standard_deviation_estimate(component2);

    double sum_corr = 0.0;
    double sum_squared_deviation_component1 = 0.0;
    double sum_squared_deviation_component2 = 0.0;

    for (int i = 0; i < _info_header.bi_height; ++i) {
        for (int j = 0; j < _info_header.bi_width; ++j) {
            double pixel_component1 = _data[(i * _info_header.bi_width + j) * 3 + component1_index];
            double pixel_component2 = _data[(i * _info_header.bi_width + j) * 3 + component2_index];

            double deviation_component1 = pixel_component1 - mean_component1;
            double deviation_component2 = pixel_component2 - mean_component2;

            sum_corr += deviation_component1 * deviation_component2;
            sum_squared_deviation_component1 += deviation_component1 * deviation_component1;
            sum_squared_deviation_component2 += deviation_component2 * deviation_component2;
        }
    }

    int W = _info_header.bi_width;
    int H = _info_header.bi_height;
    double normalization_factor = 1.0 / (W * H);

    double correlation = sum_corr * normalization_factor / (std_deviation_component1 * std_deviation_component2);

    return correlation;
}

double BMP::calculate_autocorrelation(char component, int x_shift, int y_shift) {
    if (_info_header.bi_width == 0 || _info_header.bi_height == 0) {
        throw std::runtime_error("Image dimensions are invalid.");
    }

    double mean_intensity = calculate_mathematical_expectation(component);
    int component_index = get_component_index(component);

    double sum_autocorrelation = 0.0;
    double sum_squared_deviation1 = 0.0;
    double sum_squared_deviation2 = 0.0;
    int count = 0;

    for (int i = 0; i < _info_header.bi_height; ++i) {
        for (int j = 0; j < _info_header.bi_width; ++j) {
            int neighbor_i = i + y_shift;
            int neighbor_j = j + x_shift;

            if (neighbor_i >= 0 && neighbor_i < _info_header.bi_height && neighbor_j >= 0 && neighbor_j < _info_header.bi_width) {
                double pixel_intensity1 = _data[(i * _info_header.bi_width + j) * 3 + component_index];
                double pixel_intensity2 = _data[(neighbor_i * _info_header.bi_width + neighbor_j) * 3 + component_index];

                sum_autocorrelation += (pixel_intensity1 - mean_intensity) * (pixel_intensity2 - mean_intensity);
                sum_squared_deviation1 += (pixel_intensity1 - mean_intensity) * (pixel_intensity1 - mean_intensity);
                sum_squared_deviation2 += (pixel_intensity2 - mean_intensity) * (pixel_intensity2 - mean_intensity);
                count++;
            }
        }
    }

    if (count == 0) {
        throw std::runtime_error("Shift values are too large.");
    }

    double correlation = sum_autocorrelation / sqrt(sum_squared_deviation1 * sum_squared_deviation2);

    return correlation;
}

double BMP::calculate_PSNR(const BMP& otherBMP, char component) const {
    if (_info_header.bi_width == 0 || _info_header.bi_height == 0) {
        throw std::runtime_error("Image dimensions are invalid.");
    }

    int component_index = get_component_index(component);

    double sum_squared_deviations = 0.0;

    for (int i = 0; i < _info_header.bi_height; ++i) {
        for (int j = 0; j < _info_header.bi_width; ++j) {
            double pixel_intensity_original = _data[(i * _info_header.bi_width + j) * 3 + component_index];
            double pixel_intensity_restore = otherBMP._data[(i * otherBMP._info_header.bi_width + j) * 3 + component_index];
            double deviation = pixel_intensity_original - pixel_intensity_restore;
            sum_squared_deviations += deviation * deviation;
        }
    }

    int W = _info_header.bi_width;
    int H = _info_header.bi_height;
    const int bit_count = _info_header.bi_bit_count / 3;
    double normalization_factor = W * H * pow((pow(2, bit_count) - 1), 2);

    return 10 * log10(normalization_factor / sum_squared_deviations);
}

std::map<int, double> BMP::calculate_probability(char component) const {
    std::map<int, int> count_map;
    int component_index = get_component_index(component);

    for (int i = 0; i < _info_header.bi_height; ++i) {
        for (int j = 0; j < _info_header.bi_width; ++j) {
            int pixel_intensity = _data[(i * _info_header.bi_width + j) * (_info_header.bi_bit_count / 8) + component_index];
            count_map[pixel_intensity]++;
        }
    }

    std::map<int, double> probability_map;
    int total_pixels = _info_header.bi_width * _info_header.bi_height;

    for (const auto& pair : count_map) {
        probability_map[pair.first] = static_cast<double>(pair.second) / total_pixels;
    }

    return probability_map;
}

double BMP::calculate_entropy(char component) const {
    std::map<int, double> probability_map = calculate_probability(component);
    double entropy = 0.0;

    for (const auto& pair : probability_map) {
        double p_x = pair.second;
        if (p_x > 0) {
            entropy -= p_x * log2(p_x);
        }
    }

    return entropy;
}


int BMP::get_component_index(char component) {
    int component_index;

    switch(component) {
        case 'r':
            component_index = 2; // Red component
            break;
        case 'g':
            component_index = 1; // Green component
            break;
        case 'b':
            component_index = 0; // Blue component
            break;
        default:
            throw std::invalid_argument("Invalid component specifier");
    }

    return component_index;
}










