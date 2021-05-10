#include "JSon_renderer.h"

Json::Json(size_t stat) {
    status = stat;
}

QByteArray Json::JSon_request(int file_Size, int part_size, QString file_name) {
    status = 0;
    record_Object.insert("action", 11);
    record_Object.insert("file_size", file_Size);
    record_Object.insert("part_size", part_size);
    record_Object.insert("part_num", file_Size / part_size);
    record_Object.insert("uniq", "HASH");
    record_Object.insert("file_name", file_name);
    QJsonDocument doc(record_Object);
    QString form_json = doc.toJson(QJsonDocument::Indented);
    QByteArray form = QJsonDocument(record_Object).toJson();
    return form;
}

QByteArray Json::JSon_second_request(int part_num, QString token) {
    record_Object.insert("action", 12);
    record_Object.insert("token", token);
    record_Object.insert("part_count", part_num);
    record_Object.insert("uniq", "HASH");
}
