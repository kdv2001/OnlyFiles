#include <iostream>

#include "client/JSon_renderer.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    Json json_file(1);

    QFile file_json ("first.json");
    file_json.open(QIODevice::WriteOnly);
    file_json.write(json_file.JSon_request(115, 100));
    return 0;
}
