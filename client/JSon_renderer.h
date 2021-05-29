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
    QByteArray form;
    void remove_n();

public:
    QByteArray JSon_request_11(int file_size, int part_count, QString file_name, std::string hash);
    QByteArray JSon_request_12(int part_num, QString token);
    QByteArray JSon_request_13(QString token);
    QByteArray JSon_request_21(QString file_number, QString pin);
    QByteArray JSon_request_22(QString file_number, QString pin, int part_size, int part_num, int file_size);
    QByteArray JSon_request_23(QString token, int part_num);

    Json();
    //~Json();
};

#endif //ONLYFILES_JSON_RENDERER_H