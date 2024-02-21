#include "bmp.h"
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

BMP::BMP(const std::string& fname) {
    fs::path filePath(fs::current_path().parent_path() / "res" / (fname + ".bmp"));

    std::ifstream fin(filePath, std::ios::binary);
    if (!fin) {
        throw std::runtime_error("Error: Failed to open file for reading");
    }

    fin.read(reinterpret_cast<char*>(&_file_header), sizeof(_file_header));
    if (_file_header.bf_type != BMP_MAGIC_NUMBER) {
        throw std::runtime_error("Error: Unknown file format");
    }
    fin.read(reinterpret_cast<char*>(&_info_header), sizeof(_info_header));

    size_t data_size = static_cast<size_t>(_info_header.bi_width) * _info_header.bi_height * _info_header.bi_bit_count / 8;
    _data.resize(data_size);
    fin.read(reinterpret_cast<char*>(_data.data()), static_cast<std::streamsize>(data_size));

    std::cout << "Loaded image: " << fname << ".bmp\n";
    std::cout << "Image width: " << _info_header.bi_width << " pixels\n";
    std::cout << "Image height: " << _info_header.bi_height << " pixels\n";
    std::cout << "Image bit count: " << _info_header.bi_bit_count << " bits per pixel\n";
}

void BMP::save_file(const std::string& fname) {
    save_file(fname, _data);
}

void BMP::save_file(const std::string& fname, const std::vector<uint8_t>& data) {
    fs::path fpath(fs::current_path().parent_path() / "res");
    if (!fs::is_directory(fpath)) {
        fs::create_directory(fpath);
    }
    std::ofstream fout(fpath / (fname + ".bmp"), std::ios::binary);
    if (fout) {
        fout.write(reinterpret_cast<char*>(&_file_header), sizeof(_file_header));
        fout.write(reinterpret_cast<char*>(&_info_header), sizeof(_info_header));

        auto dataSize = static_cast<std::streamsize>(data.size());
        fout.write(reinterpret_cast<char*>(const_cast<std::vector<uint8_t>&>(data).data()), dataSize);
    }
}

void BMP::save_file_by_component(const std::string& fname, const char mod) {
    std::vector<uint8_t> modifiedData = _data;
    switch (mod) {
        case RED_MODIFIER:
            for (size_t i = 0; i < modifiedData.size(); i += 3) {
                modifiedData[i] = 0x00;
                modifiedData[i + 1] = 0x00;
            }
            break;
        case GREEN_MODIFIER:
            for (size_t i = 0; i < modifiedData.size(); i += 3) {
                modifiedData[i] = 0x00;
                modifiedData[i + 2] = 0x00;
            }
            break;
        case BLUE_MODIFIER:
            for (size_t i = 0; i < modifiedData.size(); i += 3) {
                modifiedData[i + 1] = 0x00;
                modifiedData[i + 2] = 0x00;
            }
            break;
        default:
            throw std::invalid_argument("Error: Invalid modifier");
    }
    save_file(fname + "_by_" + mod + "_component", modifiedData);
}
