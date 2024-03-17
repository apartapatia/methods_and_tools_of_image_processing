#include "bmp/bmp.h"
#include <iomanip>
#include <windows.h>


void setConsoleColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void drawRectangleWithText(const std::string& text, int width, int height, int color) {
    int padding = (width - text.length()) / 2;

    setConsoleColor(color);
    std::cout << std::setfill('~') << std::setw(width + 2) << "" << std::endl;

    for (int i = 0; i < height; ++i) {
        std::cout << "~";
        if (i == height / 2) {
            setConsoleColor(color);
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

    drawRectangleWithText("point 2 - 3", 20, 1, 10);

    BMP f_1("kodim14");
    BMP::RGB_to_YCbCr(f_1.get_data(), "kodim14");
    f_1.save_file_by_component("kodim14", 'r');
    f_1.save_file_by_component("kodim14", 'g');
    f_1.save_file_by_component("kodim14", 'b');

    BMP f_ycbcr("kodim14_YCbCr_component");
    BMP::YCbCr_to_RGB(f_ycbcr.get_data(), "kodim14_YCbCr_component");
    BMP f_rbg("kodim14_YCbCr_component_RGB_from_YCbCr");
    // BMP f_2("cat");
    // f_2.save_file_by_component("cat", 'r');
    // f_2.save_file_by_component("cat", 'g');
    // f_2.save_file_by_component("cat", 'b');


    // 4 пункт
    drawRectangleWithText("point 4", 20, 1, 10);
    double mean_intensity_r_g = f_1.calculate_rgb_correlation('r', 'g');
    double mean_intensity_r_b = f_1.calculate_rgb_correlation('r', 'b');
    double mean_intensity_b_g = f_1.calculate_rgb_correlation('b', 'g');

    std::vector components = {RED_MODIFIER, GREEN_MODIFIER, BLUE_MODIFIER};
    std::vector deltaY = {-10, -5, 0, 5, 10};

    std::cout << "Mean Intensity between r and g: " << mean_intensity_r_g << std::endl;
    std::cout << "Mean Intensity between r and b: " << mean_intensity_r_b << std::endl;
    std::cout << "Mean Intensity between b and g: " << mean_intensity_b_g << std::endl;


    // TODO автокоррелляция
    drawRectangleWithText("i`m not sure that's right", 50, 1, 4);
    for (char component : components) {
        for (int delta : deltaY) {
            double autocorrelation = f_1.calculate_autocorrelation(component, 150, delta);
            std::cout << "Autocorrelation for component " << component << " with delta y = " << delta << ": " << autocorrelation << std::endl;
        }
    }

    // пункт 5  r - Cr, g - Cb, b - y
    drawRectangleWithText("point 5", 20, 1, 10);

    double mean_intensity_y_cb = f_ycbcr.calculate_rgb_correlation('b', 'g');
    double mean_intensity_y_cr = f_ycbcr.calculate_rgb_correlation('b', 'r');
    double mean_intensity_cr_cb = f_ycbcr.calculate_rgb_correlation('r', 'g');

    std::cout << "Mean Intensity between y and cb: " << mean_intensity_y_cb << std::endl;
    std::cout << "Mean Intensity between y and cr: " << mean_intensity_y_cr << std::endl;
    std::cout << "Mean Intensity between cr and cb: " << mean_intensity_cr_cb << std::endl;

    // пункт 7
    drawRectangleWithText("point 7", 20, 1, 10);
    double PSNR_B = f_1.calculate_PSNR(f_rbg, 'b');
    double PSNR_G = f_1.calculate_PSNR(f_rbg, 'g');
    double PSNR_R = f_1.calculate_PSNR(f_rbg, 'r');

    std::cout << "PSNR for component b: " << PSNR_B << std::endl;
    std::cout << "PSNR for component g: " << PSNR_G << std::endl;
    std::cout << "PSNR for component r: " << PSNR_R << std::endl;

    // дополнительное задание
    drawRectangleWithText("non main task", 20, 1, 10);
    f_1.RGB_to_YCbCr_with_bit_planes(f_1.get_data(), "kodim14");


    return 0;
}
