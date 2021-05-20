#include "main_window.h"
#include "ui_download_window_2.h"
//#include "ui_staked_windows.h"

MainWindow::MainWindow(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::Form)
{
    Json json(1);
    ui->setupUi(this);
    ui->stack->setCurrentIndex(0);
    socket = new QTcpSocket(this);
    load_window = new download_window(this);
    ui->stack->insertWidget(1, load_window);

    connect(socket, SIGNAL(readyRead()), this, SLOT(sock_ready()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(sock_disk()));
    connect(load_window, SIGNAL(return_home()), this, SLOT(move_home())); //signal swap widget
    //connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(show_widget()));
    //connect(socket, SIGNAL(readyRead()), this, SLOT(send_files()));
}

MainWindow::~MainWindow() {
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

    if (!(json_doc.object().value("token").isNull()) && !(json_doc.object().value("partstoupload").isNull())) {
        token = json_doc.object().value("token").toString();
        parts_to_upload = json_doc.object().value("partstoupload").toArray();
        action = REQUEST_PART_DOWNLOAD;

        qDebug() << "valid json status 12";
    }

    if (!(json_doc.object().value("fileNumber").isNull()) && !(json_doc.object().value("pin").isNull())){
        get_file_data = json_doc.object().value("fileNumber").toArray();
        get_file_data = json_doc.object().value("pin").toArray();
        //print_get_file_data(get_file_data);
        action = 13;

        qDebug() << "valid json status 13";
    }
    qDebug() << "sock read";
}

void MainWindow::on_pushButton_clicked() {
    action = REQUEST_DOWNLOAD;
    Json json(1);
    socket->connectToHost("95.72.43.35", 8080);
    socket->write(json.JSon_request_11(160, 16, "kdv"));
    if (!(socket->isOpen())){
        return;
    }
}

void MainWindow::on_pushButton_2_clicked() { //start send - Bottom
    Json json(1);
    get_path();
    ui->stack->setCurrentIndex(1);
    qDebug() << "swap window(upload)";
    if (action == 12) {
        socket->write(json.JSon_request_12(0, token));

    //открываем второе окно (нужно пробросить сокет и массивы и путь файла)
    //во втором окне кидаем json и байты, пока не придет пин и номер файла,
    //затем выходим в мейн виндом и Qmessagebox показываем пин номер файла
    //точно также на выгрузку
    }
}
//void MainWindow::send_files() {
//    Json json(1);
//
//    if (json_doc.object().value("status") == 0) {
//        std::cout << "Connected" << std::endl;
//        status = 12;
//    }
//    else if (json_doc.object().value("status") == 1) {
//        std::cout << "Not connected" << std::endl;
//    }
//
//    if (status == 12){
//        socket->write(json.JSon_request_12(0,token));
//    }
//
//    socket->write(json.JSon_request_12(0,token));
//
//    if (status == 13){
//
//    }
//}

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