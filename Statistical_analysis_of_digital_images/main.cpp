#include "bmp/bmp.h"

using namespace std;

int main(int argc, char* argv[]) {
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

    return 0;
}