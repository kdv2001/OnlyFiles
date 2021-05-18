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
#include <QtNetwork/QTcpSocket>
#include <QDebug>

class Json {
private:
    int stat;
    QByteArray form;

public:
    QByteArray JSon_request_11(int file_Size, int part_size, QString file_name);
    QByteArray JSon_request_12(int part_num, QString token);
    QByteArray JSon_request_13(int part_num, QString token);
    QByteArray JSon_parse();
    Json(int status);
    //~Json();
};

#endif //ONLYFILES_JSON_RENDERER_H