#ifndef ONLYFILES_TCP_CLIENT_H
#define ONLYFILES_TCP_CLIENT_H

#include "JSon_renderer.h"
#include "file_coder.h"
#include "download_window.h"
#include "file_coder.h"
#include "md5wrapper.h"
#include "md5.h"

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
#define REQUEST_UPLOAD 11 //request download file
#define REQUEST_PART_UPLOAD 12 //request download part or parts file
#define REQUEST_GET_ACCESS_DATA 13
//Part II
#define REQUEST_GET_FILE 21
#define REQUEST_DOWNLOAD_FILE 22
#define REQUEST_DOWNLOAD_PART 23

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

    QString file_name;
    QString hash;
    size_t file_size;
    size_t part_requsts;
    bool json_readed;

    QString pin; //get pin from server
    QString file_number; //get file number from server
    QString get_ui_pin; //get pin from ui
    QString get_ui_file_number; //get file number from ui
    QString path_to_file; //get path to file from ui

    QJsonArray parts_to_upload;//upload_parts
    QTcpSocket* socket;
    QByteArray data;//get json from server
    QString token;// token
    QJsonParseError json_doc_error;//json error
    QJsonDocument json_doc;//json

    int action;

    download_window *load_window;
    file_coder *file;
    Json *json;


public slots:
    void sock_ready();
    void sock_disk();
    void on_change_download_clicked();
    void on_change_upload_clicked();

    void on_start_download_clicked();
    void on_button_check_file_clicked();

    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void send_files();
    void get_path();
    void move_home();
    void set_action_cancel();
    void get_pin_from_ui();
    void get_file_number_from_ui();
    void start_download_file();
    void move_upload();

signals:
    void start_send();
    void start_download();
};


#endif //ONLYFILES_TCP_CLIENT_H
