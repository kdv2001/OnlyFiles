/********************************************************************************
** Form generated from reading UI file 'download_window.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DOWNLOAD_WINDOW_H
#define UI_DOWNLOAD_WINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_download_window
{
public:
    QProgressBar *progressBar;
    QPushButton *cancel_button;
    QPushButton *pushButton_2;

    void setupUi(QWidget *download_window)
    {
        if (download_window->objectName().isEmpty())
            download_window->setObjectName(QString::fromUtf8("download_window"));
        download_window->resize(806, 509);
        progressBar = new QProgressBar(download_window);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setGeometry(QRect(57, 140, 501, 51));
        progressBar->setValue(24);
        cancel_button = new QPushButton(download_window);
        cancel_button->setObjectName(QString::fromUtf8("cancel_button"));
        cancel_button->setGeometry(QRect(328, 350, 201, 71));
        pushButton_2 = new QPushButton(download_window);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(58, 370, 151, 61));

        retranslateUi(download_window);

        QMetaObject::connectSlotsByName(download_window);
    } // setupUi

    void retranslateUi(QWidget *download_window)
    {
        download_window->setWindowTitle(QApplication::translate("download_window", "Form", nullptr));
        cancel_button->setText(QApplication::translate("download_window", "cancel", nullptr));
        pushButton_2->setText(QApplication::translate("download_window", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class download_window: public Ui_download_window {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DOWNLOAD_WINDOW_H
