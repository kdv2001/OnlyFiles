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

    file_is_coder = 1;
    work_file.close();
    return 0;
}

size_t file_coder::get_file_part(QString token, size_t part_size, QByteArray data, int part_num) {
    qDebug() << "start download";
    get_derektory = QCoreApplication::applicationDirPath() + "/get_file/" + token;
    QDir dir(get_derektory);
    if(!dir.exists())
    {
        qDebug() << "Creating get part " << QCoreApplication::applicationDirPath() + "/get_file/" + token << "directory";
        dir.mkpath(get_derektory);
    }
    else
    {
        qDebug() << "get part " << QCoreApplication::applicationDirPath() + "/get_file/" + token << " already exists";
    }
    QFile work_file(get_derektory + "/" + QString::number(part_num)); //+ ".bin"
    if(!(work_file.open(QIODevice::WriteOnly))) {
        qDebug() << "cant found file get part";
        return -1;
    }
    qDebug() << "part num get part" << part_num;
    work_file.write(data);
    work_file.close();
    return 0;
}

size_t file_coder::linker_file(int part_count, QString file_name){
    QDir finish_dir(QCoreApplication::applicationDirPath() + "/download_files/");
    if(!finish_dir.exists())
    {
        qDebug() << "Creating get part " << QCoreApplication::applicationDirPath() + "/download_files/" << "directory";
        finish_dir.mkpath(QCoreApplication::applicationDirPath() + "/download_files/");
    }
    else
    {
        qDebug() << "get part " << QCoreApplication::applicationDirPath() + "/download_files/"  << " already exists";
    }
    QFile finish_file(QCoreApplication::applicationDirPath() + "/download_files/" + file_name);
    if(!(finish_file.open(QIODevice::WriteOnly))) {
        qDebug() << "cant found file linker";
        return -1;
    }

    for(int i = 0; i <= part_count; i++ ){
        QFile work_file(get_derektory + "/" + QString::number(i));
        if (!(work_file.open(QIODevice::ReadOnly))) {
            qDebug() << "cant found file linker";
            return -1;
        }
        QByteArray buffer;
        buffer = work_file.readAll();
        finish_file.write(buffer);
        work_file.close();
    }
    finish_file.close();
    return 0;
}

size_t file_coder::get_data_file(size_t part_count, QString path) {
    QFile work_file(path); //+ ".bin"
    if(!(work_file.open(QIODevice::ReadOnly))) {
        qDebug() << "cant found file get daata upload";
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

bool file_coder::delete_trans_derectory() {
    QDir dir(QCoreApplication::applicationDirPath() + "/trans_file/");
    dir.removeRecursively();
    return 0;
}

bool file_coder::delete_get_derectory() {
    QDir dir(QCoreApplication::applicationDirPath() + "/get_file/");
    dir.removeRecursively();
    return 0;
}

size_t file_coder::find_end_of_json(QByteArray data_string) {
    size_t opening_quotes = 0;
    size_t closing_quotes = 0;
    size_t i = 0;
    bool check_needed = false;
    while (1) {
        if(i >= data_string.length()){
            i = -1;
            break;
        }
        switch (data_string.at(i)) {
            case '{':
                ++opening_quotes;
                check_needed = true;
                break;
            case '}':
                ++closing_quotes;
                check_needed = true;
                break;
        }
        if (check_needed && closing_quotes == opening_quotes) {
            break;
        }
        ++i;
    }
    ++i;
    return i;
}