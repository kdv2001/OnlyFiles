#include "file_coder.h"

file_coder::file_coder() {

}

file_coder::~file_coder() {

}

size_t file_coder::code_file(QString token, QString path){
    if(file_is_coder == 1) {
        return 2;
    }

    QFile work_file(path); //+ ".bin"
    //std::ifstream open_file;
    if(!(work_file.open(QIODevice::ReadOnly))) {
        qDebug() << "cant found file";
        return -1;
    }
    QDataStream stream(&work_file);
    qDebug() << "file size/file name: " << file_size << " " << file_name;

    work_derektory = QCoreApplication::applicationDirPath() + "/trans_file/" + token;
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
    int count = 0;
    std::string to_string(int count);
    QByteArray buffer;

    for (size_t i = 0; i < file_size; i = i + part_size) {

        qDebug() << work_derektory + "/" + QString::number(count) + ".bin";
        QFile create_file(work_derektory + "/" + QString::number(count) + ".bin");
        create_file.open(QIODevice::WriteOnly);
        work_file.seek(i);
        QDataStream get_stream(&create_file);
        buffer = work_file.read(part_size);
        create_file.write(buffer);
        create_file.close();
        count++;
    }


//    open_file.open(path.toStdString(), std::ios::binary | std::ios::in);
//    if(!open_file) {
//       std::cout << "cant open file";
//        return -1;
//    }

//    char* get_data;
//
//    part_count = file_size / part_size;
//    for (int i = 0; i < file_size; i = i + part_size) {
//        get_data = new char[part_size];
//        count++;
//        std::ofstream new_file;
//        QString work_direct = work_derektory + "/" + QString::number(count) + ".bin";
//        new_file.open(work_direct.toStdString(), std::ios::binary | std::ios::out);
//        if ((i + part_size) > file_size){
//            open_file.read(get_data, file_size - i);
//            new_file.write(get_data, file_size - i);
//            new_file.close();
//            break;
//        }
//        open_file.read(get_data, part_size);
//        new_file.write(get_data, part_size);
//        new_file.close();
//
//    }
    file_is_coder = 1;
    work_file.close();
    return 0;
}

size_t file_coder::get_file_part(QString token, size_t part_size, QByteArray data) {
    while(data.toStdString().at(0) != '}') {
        data.toStdString().erase(0,1);
    }
    //QDir get_derektory = QCoreApplication::applicationDirPath() + "/get_file/" + token;
    QDir dir(QCoreApplication::applicationDirPath() + "/get_file/" + token);
    if(!dir.exists())
    {
        qDebug() << "Creating " << QCoreApplication::applicationDirPath() + "/get_file/" + token << "directory";
        dir.mkpath(QCoreApplication::applicationDirPath() + "/get_file/" + token);
    }
    else
    {
        qDebug() <<  QCoreApplication::applicationDirPath() + "/get_file/" + token << " already exists";
    }
    QFile work_file(QCoreApplication::applicationDirPath() + "/get_file/" + token); //+ ".bin"
    //std::ifstream open_file;
    if(!(work_file.open(QIODevice::ReadOnly))) {
        qDebug() << "cant found file";
        return -1;
    }
    QDataStream stream(&work_file);
    return 0;
}

size_t file_coder::get_data_file(size_t part_count, QString path) {
    QFile work_file(path); //+ ".bin"
    if(!(work_file.open(QIODevice::ReadOnly))) {
        qDebug() << "cant found file";
        return -1;
    }
    QFileInfo work_file_info(path);
    file_name = work_file_info.fileName();
    file_size = work_file_info.size();
    part_size = file_size / part_count;

    work_file.close();
    return 0;
}

qint64 file_coder::return_file_size() {
    return file_size;
}

size_t file_coder::return_part_size() {
    return part_size;
}

QString file_coder::return_file_name() {
    return file_name;
}

void file_coder::set_file_coder(){
    file_is_coder = 0;
}

QString file_coder::return_file_path() {
    return work_derektory;
}

bool file_coder::delete_time_derectory(QString path_to_file) {
    QDir dir(path_to_file);
    dir.removeRecursively();
    return 0;
}