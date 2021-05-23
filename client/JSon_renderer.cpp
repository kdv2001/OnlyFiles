#include "JSon_renderer.h"

Json::Json(){

}

QByteArray Json::JSon_request_11(int file_Size, int part_size, QString file_name) {
    QJsonObject record_Object;
    record_Object.insert("action", 11);
    record_Object.insert("file_size", file_Size);
    record_Object.insert("part_size", part_size);
    record_Object.insert("part_num", file_Size / part_size);
    record_Object.insert("file_name", file_name);
    record_Object.insert("uniq", "HASH");
    QJsonDocument doc(record_Object);
    QString form_json = doc.toJson(QJsonDocument::Indented);
    form = QJsonDocument(record_Object).toJson();
    return form;
}

QByteArray Json::JSon_request_12(int part_num, QString token) {
    QJsonObject record_Object;
    record_Object.insert("action", 12);
    record_Object.insert("token", token);
    record_Object.insert("part_num:", part_num);
    QJsonDocument doc(record_Object);
    QString form_json = doc.toJson(QJsonDocument::Indented);
    form = doc.toJson();
    return form;
}

QByteArray Json::JSon_request_13(int part_num, QString token) {
    QJsonObject record_Object;
    record_Object.insert("action", 12);
    record_Object.insert("token", token);
    form = QJsonDocument(record_Object).toJson();
    return form;
}