#ifndef ONLYFILES_JSON_RENDERER_H
#define ONLYFILES_JSON_RENDERER_H

#include <iostream>
#include <string>
#include <QWidget>
#include <Qt>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFileDialog>
#include <QFile>
#include <QDir>

#define OK 0; //status ok
#define NOT_OK 1; //status ne ok
#define REQUEST_DOWNLOAD 11; //request download file
#define REQUEST_PART_DOWNLOAD 12; //request download part or parts file

class Json {
private:
    int status;
public:
    QByteArray JSon_request(int file_Size, int part_size);
    Json(size_t stat);
    //~Json();
};

#endif //ONLYFILES_JSON_RENDERER_H