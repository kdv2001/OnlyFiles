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
#include <QCryptographicHash>
#include "md5wrapper.h"
#include "md5.h"

class file_coder {

public:
    file_coder();
    ~file_coder();
    size_t code_file(QString token, QString path);
    qint64 return_file_size();
    size_t return_part_size();
    QString return_file_name();
    size_t get_data_file(size_t part_count, QString path);
    void set_file_coder();
    QString return_file_path();
    bool delete_trans_derectory();
    bool delete_get_derectory();

    size_t linker_file(int part_count, QString file_name);
    size_t get_file_part(QString token, size_t part_size, QByteArray data, int part_num);
    size_t find_end_of_json(QByteArray data_string);

private:
    QString get_derektory;
    QString work_derektory;
    bool file_is_coder = 0;
    QString file_name;
    qint64 file_size;
    size_t part_count;
    size_t part_size;
};


#endif //ONLYFILES_FILE_CODER_H
