//
// Created by kdv on 17.05.2021.
//
#include "JSon_renderer.h"

#include <QtNetwork/QTcpSocket>
#include <iostream>
#include <string>
#include <QWidget>
#include <Qt>
#include <QInputDialog>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFileDialog>
#include <QFile>
#include <QDir>
#include <QMainWindow>
#include <QMessageBox>
#include <QStandardItem>
#include <QDialog>

#ifndef ONLYFILES_DOWNLOAD_WINDOW_H
#define ONLYFILES_DOWNLOAD_WINDOW_H

namespace Ui{
    class download_window;
}

class download_window: public QWidget {
    Q_OBJECT
private:
    Ui::download_window *ui;

public:
    explicit download_window(QWidget *parent);
    ~download_window();
    size_t download(QString token);
    Json *json;
    QTcpSocket *download_socket;
    QString tok;
    QByteArray data;
public slots:
    void on_cancel_button_clicked(); // cancel button
    void on_pushButton_2_clicked();
    void sock_ready();
    void sock_disk();
signals:
    void return_home(); //signal move home
};

#endif //ONLYFILES_DOWNLOAD_WINDOW_H
