#include "JSon_renderer.h"

Json::Json(size_t stat) {
    status = stat;
}

QByteArray Json::JSon_request(int file_Size, int part_size) {
    status = 0;
    QJsonObject record_Object;
    record_Object.insert("action", 11);
    record_Object.insert("file_size", file_Size);
    record_Object.insert("part_size", part_size);
    record_Object.insert("part_num", file_Size / part_size);
    record_Object.insert("uniq", "HASH");
    record_Object.insert("file_name", 1);
    QJsonDocument doc(record_Object);
    QString form_json = doc.toJson(QJsonDocument::Indented);
    QByteArray form = QJsonDocument(record_Object).toJson();
    return form;
    /*
    QFile file_json ("first.json");
    file_json.open(QIODevice::WriteOnly);
    file_json.write(form);
     */
}


