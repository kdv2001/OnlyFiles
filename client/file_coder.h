#ifndef ONLYFILES_FILE_CODER_H
#define ONLYFILES_FILE_CODER_H
#include <iostream>
#include <fstream>

class file_coder {
    file_coder();
    ~file_coder();

    size_t code_file(size_t part_size, std::string path);
};


#endif //ONLYFILES_FILE_CODER_H
