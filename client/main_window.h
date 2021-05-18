#ifndef ONLYFILES_TCP_CLIENT_H
#define ONLYFILES_TCP_CLIENT_H

#include "JSon_renderer.h"
#include "file_coder.h"
#include "download_window.h"

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
#include <QStackedWidget>

#define OK 0 //status ok
#define NOT_OK 1 //status ne ok
#define REQUEST_DOWNLOAD 11 //request download file
#define REQUEST_PART_DOWNLOAD 12 //request download part or parts file

namespace Ui{
    class Form;
}

class MainWindow : public QMainWindow{
    Q_OBJECT

private:
    Ui::Form *ui;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString path_to_file;
    QJsonArray parts_to_upload;
    QJsonArray get_file_data; //data [file number pin]
    QTcpSocket* socket;
    QByteArray data;
    QString token;
    int action;
    QJsonParseError json_doc_error;
    QJsonDocument json_doc;
    download_window *load_window;
    //QStackedWidget *stacked_widget;

public slots:
    void sock_ready();
    void sock_disk();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    //void send_files();
    void get_path();
    void print_get_file_data(QString data);
    void show_widget();

    //void sock_write();

};


#endif //ONLYFILES_TCP_CLIENT_H
