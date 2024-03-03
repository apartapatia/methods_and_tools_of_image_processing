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


BMP BMP::RGB_to_YCbCr(const std::vector<uint8_t>& rgbData, const std::string& fname) {
    BMP ycbcrBmp(fname);

    std::vector<uint8_t> yData, cbData, crData, ycbcrData;
    yData.reserve(rgbData.size());
    cbData.reserve(rgbData.size());
    crData.reserve(rgbData.size());
    ycbcrData.reserve(rgbData.size());

    for (size_t i = 0; i < rgbData.size(); i += 3) {
        uint8_t R = rgbData[i];
        uint8_t G = rgbData[i + 1];
        uint8_t B = rgbData[i + 2];

        auto Y = static_cast<uint8_t>(0.299 * R + 0.587 * G + 0.114 * B);
        auto Cb = static_cast<uint8_t>(128 + 0.5643 * (B - Y));
        auto Cr = static_cast<uint8_t>(128 + 0.7132 * (R - Y));

        yData.push_back(Y);
        ycbcrData.push_back(Y);
        cbData.push_back(Cb);
        ycbcrData.push_back(Cb);
        crData.push_back(Cr);
        ycbcrData.push_back(Cr);
    }

    ycbcrBmp.save_file(fname + "_Y_component", yData);
    ycbcrBmp.save_file(fname + "_Cb_component", cbData);
    ycbcrBmp.save_file(fname + "_Cr_component", crData);
    ycbcrBmp.save_file(fname + "_YCbCr_component", ycbcrData);

    return ycbcrBmp;
}

