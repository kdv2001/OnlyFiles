//
// Created by kdv on 17.05.2021.
//
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

public slots:
    void return_window();

    //    void sock_ready();
//    void sock_disk();
//public signals:

};


#endif //ONLYFILES_DOWNLOAD_WINDOW_H
