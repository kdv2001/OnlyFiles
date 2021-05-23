//
// Created by kdv on 17.05.2021.
//
#include "ui_download_window.h"
#include "JSon_renderer.h"
#include "download_window.h"
#include "file_coder.h"

download_window::download_window(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::download_window)

{
    json = new Json();
    ui->setupUi(this);
}

void download_window::sock_disk() {
    download_socket->deleteLater();
}

void download_window::sock_ready() {
    data = download_socket->readAll();
    qDebug() << data;
}

download_window::~download_window() {
    delete ui;
}

void download_window::on_cancel_button_clicked() {
    emit return_home();
}

void download_window::on_pushButton_2_clicked() {
}

size_t download_window::download(QString token) {
    download_socket->write(json->JSon_request_12(0, token) + "40340329493240");
    tok = token;
    return 0;
}
