#ifndef ONLYFILES_MANGER_H
#define ONLYFILES_MANGER_H
#include "JSon_renderer.h"

class Manger {
public:
    Manger(){
        Json json_file(1);

        QFile file_json ("first.json");
        file_json.open(QIODevice::WriteOnly);
        file_json.write(json_file.JSon_request(115, 100, "dfd34343443f"));
        QFile file_json2 ("second.json");
        file_json2.open(QIODevice::WriteOnly);
        file_json2.write(json_file.JSon_second_request(1, "d4343434fdfd"));
    }

};


#endif //ONLYFILES_MANGER_H
