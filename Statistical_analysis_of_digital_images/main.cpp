#include "bmp/bmp.h"
#include <iomanip>
#include <windows.h>

void setConsoleColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void drawRectangleWithText(const std::string& text, int width, int height) {
    int padding = (width - text.length()) / 2;

    setConsoleColor(10);
    std::cout << std::setfill('~') << std::setw(width + 2) << "" << std::endl;

    for (int i = 0; i < height; ++i) {
        std::cout << "~";
        if (i == height / 2) {
            setConsoleColor(10);
            std::cout << std::setw(padding + 1) << "" << text << std::setw(padding) << "";
        } else {
            std::cout << std::setw(width) << "";
        }
        std::cout << "~" << std::endl;
    }

    std::cout << std::setfill('~') << std::setw(width + 2) << "" << std::endl;

    setConsoleColor(7);
}

int main() {
    // // 2 - 3 пункты
    // BMP f("catcat");
    // f.save_file_by_component("catcat", 'r');
    // f.save_file_by_component("catcat", 'g');
    // f.save_file_by_component("catcat", 'b');

    drawRectangleWithText("point 2 - 3", 20, 1);

    BMP f_1("kodim14");
    f_1.save_file_by_component("kodim14", 'r');
    f_1.save_file_by_component("kodim14", 'g');
    f_1.save_file_by_component("kodim14", 'b');
    BMP f_ycbcr = BMP::RGB_to_YCbCr(f_1.get_data(), "kodim14");

    // BMP f_2("cat");
    // f_2.save_file_by_component("cat", 'r');
    // f_2.save_file_by_component("cat", 'g');
    // f_2.save_file_by_component("cat", 'b');


    // 4 пункт
    drawRectangleWithText("point 4", 20, 1);
    double mean_intensity_r_g = f_1.calculate_rgb_correlation('r', 'g');
    double mean_intensity_r_b = f_1.calculate_rgb_correlation('r', 'b');
    double mean_intensity_b_g = f_1.calculate_rgb_correlation('b', 'g');

    std::vector components = {RED_MODIFIER, GREEN_MODIFIER, BLUE_MODIFIER};
    std::vector deltaY = {-10, -5, 0, 5, 10};

    std::cout << "Mean Intensity between r and g: " << mean_intensity_r_g << std::endl;
    std::cout << "Mean Intensity between r and b: " << mean_intensity_r_b << std::endl;
    std::cout << "Mean Intensity between b and g: " << mean_intensity_b_g << std::endl;

    for (char component : components) {
        for (int delta : deltaY) {
            double autocorrelation = f_1.calculate_autocorrelation(component, 150, delta);
            std::cout << "Autocorrelation for component " << component << " with delta y = " << delta << ": " << autocorrelation << std::endl;
        }
    }

    // TODO 5 пункт r - y, g - Cb, b - Cr
    drawRectangleWithText("point 5", 20, 1);


    return 0;
}
