#include "file_coder.h"

size_t file_coder::code_file(size_t part_size, std::string path){
    long file_size;
    fi.seekg(0, ios::end);
    file_size = fi.tellg();
    std::fstream(path);


}