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
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_download_window
{
public:
    QPushButton *cancel_button;
    QPushButton *pushButton_2;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QProgressBar *progressBar;

    void setupUi(QWidget *download_window)
    {
        if (download_window->objectName().isEmpty())
            download_window->setObjectName(QString::fromUtf8("download_window"));
        download_window->resize(639, 461);
        cancel_button = new QPushButton(download_window);
        cancel_button->setObjectName(QString::fromUtf8("cancel_button"));
        cancel_button->setGeometry(QRect(390, 330, 201, 71));
        pushButton_2 = new QPushButton(download_window);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(40, 330, 331, 71));
        verticalLayoutWidget = new QWidget(download_window);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(30, 80, 591, 141));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        progressBar = new QProgressBar(verticalLayoutWidget);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setEnabled(true);
        progressBar->setMinimumSize(QSize(0, 46));
        progressBar->setValue(24);

        verticalLayout->addWidget(progressBar);


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
