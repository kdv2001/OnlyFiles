#include "main_window.h"

#include <iostream>
#include <QApplication>
#include <QCryptographicHash>

int main(int argc, char *argv[]) {
    std::cout << "Hello, World!" << std::endl;
    QApplication qap(argc, argv);
    MainWindow tcp;
    tcp.show();
    return qap.exec();
}
