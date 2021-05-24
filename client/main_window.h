#ifndef ONLYFILES_TCP_CLIENT_H
#define ONLYFILES_TCP_CLIENT_H

#include "JSon_renderer.h"
#include "file_coder.h"
#include "download_window.h"
#include "file_coder.h"

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
#include <QBoxLayout>
#include <QThread>

#define OK 0 //status ok
#define NOT_OK 1 //status ne ok
#define REQUEST_DOWNLOAD 11 //request download file
#define REQUEST_PART_DOWNLOAD 12 //request download part or parts file

namespace Ui{
    class Form;
}

class MainWindow : public QWidget{
    Q_OBJECT

private:
    Ui::Form *ui;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QString pin;
    QString file_number;
    QString path_to_file;
    QJsonArray parts_to_upload;
    QTcpSocket* socket;
    QByteArray data;
    QString token;
    int action = 11;
    QJsonParseError json_doc_error;
    QJsonDocument json_doc;
    download_window *load_window;
    file_coder *file;
    Json *json;
    size_t part_size;

public slots:
    void sock_ready();
    void sock_disk();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void send_files();
    void get_path();
    void print_get_file_data(QString data);
    void move_home();
    //void sock_write();
signals:
    void start_send();
};


#endif //ONLYFILES_TCP_CLIENT_H
