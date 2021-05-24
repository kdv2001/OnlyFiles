#include "main_window.h"
#include "ui_download_window_2.h"
//#include "ui_staked_windows.h"

MainWindow::MainWindow(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::Form)
{
    ///***
    part_size = 1000;
    ///***

    file = new file_coder();
    json = new Json();
    ui->setupUi(this);
    ui->stack->setCurrentIndex(0);
    socket = new QTcpSocket(this);
    load_window = new download_window(this);
    ui->stack->insertWidget(1, load_window);
    //ui->stack->horizontalSlider
    connect(socket, SIGNAL(readyRead()), this, SLOT(sock_ready()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(sock_disk()));
    connect(load_window, SIGNAL(return_home()), this, SLOT(move_home())); //signal swap widget
    connect(this, SIGNAL(start_send()), this, SLOT(send_files()));
    connect(load_window, SIGNAL(return_home()), this, SLOT(move_home()));

}

MainWindow::~MainWindow() {
    delete load_window;
    delete file;
    delete json;
    delete ui;
}

void MainWindow::sock_disk() {
    socket->deleteLater();
}

void MainWindow::sock_ready() {
    data = socket->readAll();
    qDebug() << data << " " << action;
    json_doc = QJsonDocument::fromJson(data, &json_doc_error);
    QThread::sleep(1);

    if (json_doc.object().value("status").toInt() > 0) {
        ui->stack->setCurrentIndex(0);
        QMessageBox::warning(this, "bad status", "get status: " + QString::number((json_doc.object().value("status").toInt())));

        return;
    }

    if((action == REQUEST_DOWNLOAD) && !(json_doc.object().value("token").isNull())) {
        QMessageBox::information(this, "connection", "connect");
    }

    if((action == REQUEST_DOWNLOAD) && !(json_doc.object().value("token").isNull())){
        token = json_doc.object().value("token").toString();
        action = REQUEST_PART_DOWNLOAD;
    }

    if ((action == 13) && (!(json_doc.object().value("pin").isNull()))){

        file_number = json_doc.object().value("file_number").toString();
        pin = json_doc.object().value("pin").toString();
        ui->stack->setCurrentIndex(0);
        QMessageBox::information(this, "acces inf", "pin: " + pin + "\n"+ "file number: " + file_number);
        //print_get_file_data(get_file_data);
        action = REQUEST_DOWNLOAD;
        qDebug() << "valid json status 13";
        return;
    }
    else if ((action == 13) && ((json_doc.object().value("pin").isNull()))) {
        QMessageBox::warning(this, "", "cant get pin\nand file number");
        ui->stack->setCurrentIndex(0);
        return;
    }

    if (!(json_doc.object().value("partstoupload") == 0) && (action == REQUEST_PART_DOWNLOAD)) {

        parts_to_upload = json_doc.object().value("partstoupload").toArray();

        if (parts_to_upload.size() == 0) {
            socket->write(json->JSon_request_13(token));
            qDebug() << json->JSon_request_13(token);
            action = 13;
            qDebug() << "pizda";
            return;
        }
        file->code_file(token, part_size, path_to_file);

        qDebug() << "valid json status 12";
        emit start_send();
    }
}

void MainWindow::on_pushButton_clicked() {

}

void MainWindow::on_pushButton_2_clicked() { //start send - Bottom
    action = REQUEST_DOWNLOAD;
    get_path();
    if ((file->get_data_file(part_size, path_to_file)) == -1) {
        return;
    }
    if (!(socket->isOpen())) {
        socket->connectToHost("95.72.34.202", 8080);
    }
    socket->write(json->JSon_request_11(file->return_file_size(), part_size, file->return_file_name()));
    ui->stack->setCurrentIndex(1);
}

void MainWindow::send_files() {
    QFile send_file("/home/kdv/OF/trans_file/" + token + "/" + QString::number(parts_to_upload.at(0).toInt()) + ".bin");
    if(!(send_file.open(QIODevice::ReadOnly))) {
        qWarning() << "cant open file to send  " << "/home/kdv/OF/trans_file/" + token + "/" + QString::number(parts_to_upload.at(0).toInt())+ ".bin";
        return;
    }
    QDataStream send_file_stream(&send_file);
    QByteArray buffer;
    buffer = send_file.readAll();

    qDebug() << json->JSon_request_12(parts_to_upload.at(0).toInt(), token) + buffer;
    load_window->set_progress_bar(parts_to_upload, file->return_file_size(), part_size);
    socket->write(json->JSon_request_12(parts_to_upload.at(0).toInt(), token) + buffer);
    //qDebug() << json->JSon_request_12(parts_to_upload.at(0).toInt(), token);

}

void MainWindow::get_path() {
    path_to_file = ui->file_path->text();
}
void MainWindow::print_get_file_data(QString data) {
    //ui->label->setText(data);
}

void MainWindow::move_home() {
    ui->stack->setCurrentIndex(0);
    qDebug() << "swap window(main)";
}
