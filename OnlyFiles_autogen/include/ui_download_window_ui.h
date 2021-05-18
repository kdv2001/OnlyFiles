/********************************************************************************
** Form generated from reading UI file 'download_window_ui.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DOWNLOAD_WINDOW_UI_H
#define UI_DOWNLOAD_WINDOW_UI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_download_window
{
public:
    QProgressBar *progressBar;
    QPushButton *cancel_button;
    QPushButton *continue_button;

    void setupUi(QDialog *download_window)
    {
        if (download_window->objectName().isEmpty())
            download_window->setObjectName(QString::fromUtf8("download_window"));
        download_window->resize(806, 590);
        progressBar = new QProgressBar(download_window);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setGeometry(QRect(150, 170, 511, 41));
        progressBar->setValue(24);
        cancel_button = new QPushButton(download_window);
        cancel_button->setObjectName(QString::fromUtf8("cancel_button"));
        cancel_button->setGeometry(QRect(480, 440, 191, 61));
        continue_button = new QPushButton(download_window);
        continue_button->setObjectName(QString::fromUtf8("continue_button"));
        continue_button->setGeometry(QRect(260, 440, 191, 61));

        retranslateUi(download_window);
        QObject::connect(cancel_button, SIGNAL(clicked()), download_window, SLOT(reject()));

        QMetaObject::connectSlotsByName(download_window);
    } // setupUi

    void retranslateUi(QDialog *download_window)
    {
        download_window->setWindowTitle(QApplication::translate("download_window", "Dialog", nullptr));
        cancel_button->setText(QApplication::translate("download_window", "Cancel", nullptr));
        continue_button->setText(QApplication::translate("download_window", "Continue dowload", nullptr));
    } // retranslateUi

};

namespace Ui {
    class download_window: public Ui_download_window {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DOWNLOAD_WINDOW_UI_H
