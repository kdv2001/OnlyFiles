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
    qDebug() << data;
    json_doc = QJsonDocument::fromJson(data, &json_doc_error);

    if((action == REQUEST_DOWNLOAD) && !(json_doc.object().value("token").isNull())) {
        QMessageBox::information(this, "connection", "connect");
    }

    if ((json_doc.object().contains("token")) && !(json_doc.object().value("partstoupload") == 0)) {
        token = json_doc.object().value("token").toString();
        parts_to_upload = json_doc.object().value("partstoupload").toArray();
        action = REQUEST_PART_DOWNLOAD;
        qDebug() << "valid json status 12";
        file->code_file(token, part_size, path_to_file);
        qDebug() << action;

        qDebug() << "valid json status 12 auf";
        emit start_send();
    }

    if ((json_doc.object().value("partstoupload") == 0) && !(json_doc.object().value("pin") == 0)){
        get_file_data = json_doc.object().value("partstoupload").toArray();
        get_file_data = json_doc.object().value("pin").toArray();
        //print_get_file_data(get_file_data);
        action = 13;

        qDebug() << "valid json status 13";
    }
    qDebug() << "sock read";
}

void MainWindow::on_pushButton_clicked() {
    socket->write(json->JSon_request_11(file->return_file_size(), part_size, file->return_file_name()));
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
    qDebug() << "compile Json: "
             << json->JSon_request_11(file->return_file_size(), part_size, file->return_file_name());

    socket->write(json->JSon_request_11(file->return_file_size(), part_size, file->return_file_name()));
    ui->stack->setCurrentIndex(1);

    qDebug() << "swap window(upload)";
}

void MainWindow::send_files() {
//    if(parts_to_upload.isEmpty()){
//        return;
//    }
    qDebug() << parts_to_upload.at(0).toInt();
    QFile send_file("/home/kdv/OnlyFiles/transport_files/" + token + "/" + QString::number(parts_to_upload.at(0).toInt()) + ".bin");
    if(!(send_file.open(QIODevice::ReadOnly))) {
        qWarning() << "cant open file to send  " << "/home/kdv/OnlyFiles/transport_files/" + token + "/" + QString::number(parts_to_upload.at(0).toInt())+ ".bin";
        return;
    }
    QDataStream send_file_stream(&send_file);
    QByteArray buffer;
    buffer = send_file.readAll();

    socket->write(json->JSon_request_12(parts_to_upload.at(0).toInt(), token) + buffer);
    qDebug() << json->JSon_request_12(parts_to_upload.at(0).toInt(), token);

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

void MainWindow::open_soket() {
    socket = new QTcpSocket(this);
    socket->connectToHost("95.72.34.202", 8080);
}