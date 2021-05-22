#include "file_coder.h"

file_coder::file_coder() {

}

file_coder::~file_coder() {

}

size_t file_coder::code_file(QString token, size_t part_size, QString path){
    QFileInfo work_file_info(path);
    QFile work_file(path); //+ ".bin"
    if(!(work_file.open(QIODevice::ReadOnly))) {
        qDebug() << "cant found file";
        return -1;
    }
    QDataStream stream(&work_file);
    file_name = work_file_info.fileName();
    file_size = work_file_info.size();
    part_count = file_size / part_size;
    qDebug() << "file size/fiel name: " << file_size << " " << file_name;

    QString work_derektory = "/home/kdv/OnlyFiles/transport_files/" + token;
    QDir dir(work_derektory);
    if(!dir.exists())
    {
        qDebug() << "Creating " << work_derektory << "directory";
        dir.mkpath(work_derektory);
    }
    else
    {
        qDebug() <<  work_derektory << " already exists";
    }

    for (size_t i = part_size; i < file_size; i = i + part_size) {
        QFile create_file(work_derektory + static_cast<char>(i));
        create_file.open(QIODevice::WriteOnly);
        QDataStream get_stream(&create_file);
        get_stream << stream;
    }

//    std::fstream open_file;
//    open_file.open(path.toStdString(), std::ios::binary | std::ios::in);
//    if(!open_file) {
//        printf("%s","Error opening file");
//        return -1;
//    }
//    open_file.seekg(0, std::ios::end); //указатель на конец файла
//    unsigned long file_size = open_file.tellg();
//    part_count = file_size / part_size;
//    for (int i = 0, i < part_count, i++) {}

    return 0;
}

long file_coder::return_file_size() {
    return file_size;
}

size_t file_coder::return_part_count() {
    return part_count;
}

QString file_coder::return_file_name() {
    return file_name;
}