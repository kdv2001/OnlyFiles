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

    if (!(json_doc.object().value("token").toInt() == 1) && !(json_doc.object().value("partstoupload").isNull())) {
        token = json_doc.object().value("token").toString();
        parts_to_upload = json_doc.object().value("partstoupload").toArray();
        action = REQUEST_PART_DOWNLOAD;


        qDebug() << "valid json status 12";
    }

    if (!(json_doc.object().value("fileNumber").toInt() == 0) && !(json_doc.object().value("pin").isNull())){
        get_file_data = json_doc.object().value("fileNumber").toArray();
        get_file_data = json_doc.object().value("pin").toArray();
        //print_get_file_data(get_file_data);
        action = 13;

        qDebug() << "valid json status 13";
    }
    qDebug() << "sock read";
}

void MainWindow::on_pushButton_clicked() {
}

void MainWindow::on_pushButton_2_clicked() { //start send - Bottom
    action = REQUEST_DOWNLOAD;
    get_path();
    if((file->get_data_file(part_size, path_to_file)) == -1){
        return;
    }

    socket->connectToHost("95.72.34.202", 8080);

    qDebug() << "compile Json: " << json->JSon_request_11(file->return_file_size(), part_size, file->return_file_name());

    socket->write(json->JSon_request_11(file->return_file_size(), part_size, file->return_file_name()));
    file->code_file("8888", part_size, path_to_file);

    qDebug() << token;

    ui->stack->setCurrentIndex(1);
    qDebug() << "swap window(upload)";


}
void MainWindow::send_files() {

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