#ifndef ONLYFILES_FILE_CODER_H
#define ONLYFILES_FILE_CODER_H
#include <iostream>
#include <fstream>
#include <QCoreApplication>
#include <QDir>
#include <QDebug>
#include <QString>
#include <QDataStream>
#include <string>

class file_coder {

public:
    file_coder();
    ~file_coder();
    size_t code_file(QString token, size_t part_size, QString path);
    qint64 return_file_size();
    size_t return_part_count();
    QString return_file_name();
    size_t get_data_file(size_t part_size, QString path);
private:
    bool file_is_coder = 0;
    QString file_name;
    qint64 file_size;
    size_t status;
    size_t part_count;
};


#endif //ONLYFILES_FILE_CODER_H
