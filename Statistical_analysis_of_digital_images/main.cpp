#include "bmp/bmp.h"
#include "test/correlation.h"

using namespace std;

int main(int argc, char* argv[]) {

    // 2 - 3 пункты
    BMP f("catcat");
    f.save_file_by_component("catcat", 'r');
    f.save_file_by_component("catcat", 'g');
    f.save_file_by_component("catcat", 'b');

    BMP f_1("kodim14");
    f_1.save_file_by_component("kodim14", 'r');
    f_1.save_file_by_component("kodim14", 'g');
    f_1.save_file_by_component("kodim14", 'b');

    BMP f_2("cat");
    f_2.save_file_by_component("cat", 'r');
    f_2.save_file_by_component("cat", 'g');
    f_2.save_file_by_component("cat", 'b');


    // 4 пункт
    double mean_intensity_r_g = f_2.calculate_rgb_correlation('r', 'g');
    double mean_intensity_r_b = f_2.calculate_rgb_correlation('r', 'b');
    double mean_intensity_b_g = f_2.calculate_rgb_correlation('b', 'g');
    std::cout << "Mean Intensity between r and g: " << mean_intensity_r_g << std::endl;
    std::cout << "Mean Intensity between r and b: " << mean_intensity_r_b << std::endl;
    std::cout << "Mean Intensity between b and g: " << mean_intensity_b_g << std::endl;

    return 0;
}