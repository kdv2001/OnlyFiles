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
    ui->setupUi(this);
}

download_window::~download_window() {
    delete ui;
}

void download_window::on_cancel_button_clicked() {
    emit return_home();
}


void download_window::set_progress_bar(int parts, size_t part_count) {
    double coef = static_cast<double>(parts)/static_cast<double>(part_count);
    size_t file_progress = coef * 100;

    ui->progressBar->setValue(file_progress);
}
