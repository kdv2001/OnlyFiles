#include "main_window.h"
#include "ui_download_window_2.h"
//#include "ui_staked_windows.h"

MainWindow::MainWindow(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::Form)
{
    file = new file_coder();
    json = new Json();

    ui->setupUi(this);

    this->setFixedSize(640, 460);

    ui->stack->setCurrentIndex(0);
    socket = new QTcpSocket(this);
    load_window = new download_window(this);
    ui->stack->insertWidget(1, load_window);

    ui->lineEdit_ip->setText("95.72.35.193");

    connect(socket, SIGNAL(readyRead()), this, SLOT(sock_ready()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(sock_disk()));
    connect(load_window, SIGNAL(return_home()), this, SLOT( move_upload())); //signal swap widget
    connect(load_window, SIGNAL(return_home()), this, SLOT(set_action_cancel()));
    connect(this, SIGNAL(start_send()), this, SLOT(send_files()));
    connect(ui->cancel_button_from_upload, SIGNAL(clicked()), this, SLOT(move_home()));
    connect(ui->cancel_button_from_download, SIGNAL(clicked()), this, SLOT(move_home())); //signal swap widget
    connect(this, SIGNAL(start_download()), this, SLOT(start_download_file()));
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

    data.push_back(socket->readAll());
    //qDebug() << data.size();
    //qDebug() << "first data: " << data << " " << action;
    if (!json_readed) {
        QByteArray buffer = data;
        size_t end_of_json = file->find_end_of_json(buffer);
        buffer.remove(end_of_json, buffer.size() - end_of_json);
        data.remove(0, end_of_json);
        //qDebug() << "Buffer " << buffer;
        //qDebug() << "data " << data;
        //qDebug() << buffer;
        json_doc = QJsonDocument::fromJson(buffer, &json_doc_error);
        json_readed = true;
    }
    if (json_doc.object().value("part_size").toInt() != data.size() && action == 23) {
        return;
    }
    json_readed = false;
    //QThread::sleep(1);
    if (json_doc.object().value("status").toInt() > 0) {
        if (json_doc.object().value("status").toInt() == 1) {
            QMessageBox::warning(this, "bad status",
                                 "get status: " + QString::number((json_doc.object().value("status").toInt())) + "Not ok");
        }
        if (json_doc.object().value("status").toInt() == 2) {
            QMessageBox::warning(this, "part already uploaded/downloaded",
                                 "get status: " + QString::number((json_doc.object().value("status").toInt())));
        }
        if (json_doc.object().value("status").toInt() == 3) {
            QMessageBox::warning(this, "time out",
                                 "get status: " + QString::number((json_doc.object().value("status").toInt())));
        }
        if (json_doc.object().value("status").toInt() == 4) {
            QMessageBox::warning(this, "file not found",
                                 "get status: " + QString::number((json_doc.object().value("status").toInt())));
        }
        action = 0;
        return;
    }

    if ((action == REQUEST_UPLOAD) && !(json_doc.object().value("token").isNull())) {
        QMessageBox::information(this, "connection", "connect");
    }

    if ((action == REQUEST_UPLOAD) && !(json_doc.object().value("token").isNull())){
        token = json_doc.object().value("token").toString();
        action = REQUEST_PART_UPLOAD;
    }

    if ((action == REQUEST_GET_ACCESS_DATA) && (!(json_doc.object().value("pin").isNull()))){
        file->set_file_coder();
        file_number = json_doc.object().value("file_number").toString();
        pin = json_doc.object().value("secret").toString();
        ui->stack->setCurrentWidget(ui->page_3);
        QMessageBox::information(this, "acces inf", "file number: " + file_number + + "\n" + "pin: " + pin);
        action = REQUEST_UPLOAD;
        qDebug() << "valid json status 13";
        file->delete_trans_derectory();
        return;
    }
    else if ((action == REQUEST_GET_ACCESS_DATA) && ((json_doc.object().value("pin").isNull()))) {
        QMessageBox::warning(this, "", "cant get pin\nand file number");
        ui->stack->setCurrentIndex(0);
        file->set_file_coder();
        return;
    }

    if (!(json_doc.object().value("partstoupload") == 0) && (action == REQUEST_PART_UPLOAD)) {

        parts_to_upload = json_doc.object().value("partstoupload").toArray();

        if (parts_to_upload.size() == 0) {
            socket->write(json->JSon_request_13(token));
            qDebug() << json->JSon_request_13(token);
            action = REQUEST_GET_ACCESS_DATA;
            qDebug() << "";
            return;
        }
        if (action == 0){
            qDebug() << "action = 0";
            return;
        }
        file->code_file(token, path_to_file);
        qDebug() << "valid json status 12";
        emit start_send();
    }
    ////////////part II

    if ((action == REQUEST_DOWNLOAD_PART)) {
        parts_to_upload = json_doc.object().value("partstoupload").toArray();

        file->get_file_part(token,file_size/(ui->horizontalSlider_2->value()), data, part_requsts);
        if (parts_to_upload.size() == 0) {
            file->linker_file(ui->horizontalSlider_2->value(), file_name);
            data.clear();
            QMessageBox::information(this, "file", "file download");
            action = 0;
            part_requsts = 0;
            data.clear();
            ui->stack->setCurrentWidget(ui->page_2);
            file->delete_get_derectory();
            json_readed = false;
            return;
        }
        load_window->set_progress_bar(parts_to_upload.at(0).toInt(), ui->horizontalSlider_2->value());
        if (action == 0){
            action = 0;
            data.clear();
            qDebug() << "action = 0";
            return;
        }
        socket->write(json->JSon_request_23(token, parts_to_upload.at(0).toInt()));
        part_requsts = parts_to_upload.at(0).toInt();
        //qDebug() << socket->write(json->JSon_request_23(token, parts_to_upload.at(0).toInt())) << "what";
        qDebug() << "valid json 23";
        data.clear();
        json_readed = false;
    }

    if ((action == REQUEST_DOWNLOAD_FILE) && !(json_doc.object().value("token").isNull())) {
        token = json_doc.object().value("token").toString();
        parts_to_upload = json_doc.object().value("partstoupload").toArray();

        action = REQUEST_DOWNLOAD_PART;
        part_requsts = parts_to_upload.at(0).toInt();
        if (action == 0){
            action = 0;
            data.clear();
            qDebug() << "action = 0";
            return;
        }
        socket->write(json->JSon_request_23(token, part_requsts));
        qDebug() << json->JSon_request_23(token, parts_to_upload.at(0).toInt()) << "first 23";
        data.clear();
        json_readed = false;
    }

    if (((action == REQUEST_GET_FILE) && (!(json_doc.object().value("file_name").isNull()))) || (action == 214)) {
        file_name = json_doc.object().value("file_name").toString();
        hash = json_doc.object().value("uniq").toString();
        file_size = json_doc.object().value("file_size").toInt();
        //ui->stack->setCurrentWidget(ui->page_2);
        qDebug() << "valid json 22";
        if (action == 214) {
            ui->label_file_name->setText(file_name);
            action = 0;
            data.clear();

            json_readed = false;
            return;
        }
        action = REQUEST_DOWNLOAD_FILE;
        if (action == 0){
            action = 0;
            data.clear();
            qDebug() << "action = 0";
            return;
        }
        socket->write(json->JSon_request_22(get_ui_file_number, get_ui_pin,file_size/(ui->horizontalSlider_2->value()),ui->horizontalSlider_2->value(), file_size));

        qDebug() << json->JSon_request_22(get_ui_file_number, get_ui_pin,file_size/(ui->horizontalSlider_2->value()),ui->horizontalSlider_2->value(), file_size);
        data.clear();
        json_readed = false;
    }
}

void MainWindow::on_change_upload_clicked() {
    ui->stack->setCurrentWidget(ui->page_3);
}

void MainWindow::on_change_download_clicked() {
    ui->stack->setCurrentWidget(ui->page_2);
}
void MainWindow::on_button_check_file_clicked() {
    get_pin_from_ui();
    get_file_number_from_ui();
    if (get_ui_file_number.isEmpty() || get_ui_pin.isEmpty()) {
        QMessageBox::information(this, "message", "fill field");
        return;
    }

    if (!(socket->isOpen())) {
        socket->connectToHost(ui->lineEdit_ip->text(), 8080);
    }
    socket->write(json->JSon_request_21(get_ui_file_number, get_ui_pin));
    action = 214;
}

void MainWindow::on_start_download_clicked() { //start download file from server
    get_pin_from_ui();
    get_file_number_from_ui();
    if (get_ui_file_number.isEmpty() || get_ui_pin.isEmpty()) {
        QMessageBox::information(this, "message", "fill field");
        return;
    }
    if (!(socket->isOpen())) {
        socket->connectToHost(ui->lineEdit_ip->text(), 8080);
    }
    //check it//check it//check it//check it//check it//check it//check it//check it
//    if(QAbstractSocket::ConnectedState != socket->state()) {
//        QMessageBox::warning(this, "connect", "cant connect");
//        return;
//    }
    action = REQUEST_GET_FILE;
    socket->write(json->JSon_request_21(get_ui_file_number, get_ui_pin));
    ui->stack->setCurrentIndex(1);
}

void MainWindow::on_pushButton_2_clicked() { //start send - Bottom
    get_path();

    if (path_to_file.isEmpty()){
        QMessageBox::information(this, "file", "choose file");
        return;
    }
    md5wrapper md5;
    if ((file->get_data_file(ui->horizontalSlider->value(), path_to_file)) == -1) {
        QMessageBox::warning(this, "file", "cant found file");
        return;
    }
    if (!(socket->isOpen())) {
        socket->connectToHost(ui->lineEdit_ip->text(), 8080);
    }

    //check it//check it//check it//check it//check it//check it//check it//check it
//    if (socket->state() != QAbstractSocket::ConnectedState) {
//        QMessageBox::warning(this, "connect", "cant connect");
//        socket->close();
//        return;
//    }

    action = REQUEST_UPLOAD;
    socket->write(json->JSon_request_11(file->return_file_size(), ui->horizontalSlider->value(), file->return_file_name(), md5.getHashFromFile(path_to_file.toStdString())));

    qDebug() << json->JSon_request_11(file->return_file_size(), ui->horizontalSlider->value(), file->return_file_name(), md5.getHashFromFile(path_to_file.toStdString()));
    ui->stack->setCurrentIndex(1);
    ui->file_path->clear();
}

void MainWindow::send_files() {
    if (action == 0){
        qDebug() << "action = 0";
        return;
    }
    QFile send_file(file->return_file_path() + "/" + QString::number(parts_to_upload.at(0).toInt()) + ".bin");
    if(!(send_file.open(QIODevice::ReadOnly))) {
        qWarning() << "cant open file to send  " << file->return_file_path() + "/" + QString::number(parts_to_upload.at(0).toInt()) + ".bin";
        send_file.close();
        action = 0;
        return;
    }
    QDataStream send_file_stream(&send_file);
    QByteArray buffer;
    buffer = send_file.readAll();

    //qDebug() << json->JSon_request_12(parts_to_upload.at(0).toInt(), token) + buffer;

    load_window->set_progress_bar(parts_to_upload.at(0).toInt(), ui->horizontalSlider->value());
    if (action == 0){
        qDebug() << "action = 0";
        return;
    }
    socket->write(json->JSon_request_12(parts_to_upload.at(0).toInt(), token) + buffer);
    send_file.close();
    action = REQUEST_PART_UPLOAD;
}

//??????
void MainWindow::start_download_file() {

}

void MainWindow::get_path() {
    path_to_file = ui->file_path->text();
}

void MainWindow::move_home() {
    ui->stack->setCurrentIndex(0);
    qDebug() << "swap window(main)";
}

void MainWindow::on_pushButton_3_clicked() {
    path_to_file = QFileDialog::getOpenFileName(this, "path to file", "kdv/");
    ui->file_path->setText(path_to_file);
    qDebug() << path_to_file;
}

void MainWindow::set_action_cancel() {
    qDebug() << "action = 0";
    action = 0;
    json_readed = false;
    data.clear();
    part_requsts = 0;
}

void MainWindow::get_pin_from_ui() {
    get_ui_pin = ui->line_pin->text();
}

void MainWindow::get_file_number_from_ui() {
    get_ui_file_number = ui->line_file_number->text();
}

void MainWindow::move_upload() {
    ui->stack->setCurrentWidget(ui->page);
}
