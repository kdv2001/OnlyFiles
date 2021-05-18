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
    Json json(1);
    ui->setupUi(this);
    //socket = new QTcpSocket(this);
    //connect(socket, SIGNAL(readyRead()), this, SLOT(sock_ready()));
    //connect(socket, SIGNAL(disconnected()), this, SLOT(sock_disk()));
    connect(ui->cancel_button, SIGNAL(clicked()), this, SLOT(return_window()));
}

download_window::~download_window() {
    delete ui;
}

void download_window::return_window() {
    //emit move_hole;
}