/********************************************************************************
** Form generated from reading UI file 'download_window_2.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DOWNLOAD_WINDOW_2_H
#define UI_DOWNLOAD_WINDOW_2_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QStackedWidget *stack;
    QWidget *page;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QTextEdit *textEdit;
    QLineEdit *file_path;
    QSlider *horizontalSlider;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QString::fromUtf8("Form"));
        Form->resize(640, 460);
        stack = new QStackedWidget(Form);
        stack->setObjectName(QString::fromUtf8("stack"));
        stack->setGeometry(QRect(0, -10, 641, 461));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        pushButton = new QPushButton(page);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(30, 330, 191, 71));
        pushButton->setMouseTracking(true);
        pushButton_2 = new QPushButton(page);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(250, 330, 361, 71));
        textEdit = new QTextEdit(page);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(20, 80, 151, 51));
        file_path = new QLineEdit(page);
        file_path->setObjectName(QString::fromUtf8("file_path"));
        file_path->setGeometry(QRect(170, 80, 431, 51));
        horizontalSlider = new QSlider(page);
        horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
        horizontalSlider->setGeometry(QRect(20, 160, 261, 51));
        horizontalSlider->setMaximum(99);
        horizontalSlider->setOrientation(Qt::Horizontal);
        stack->addWidget(page);

        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "Form", nullptr));
        pushButton->setText(QApplication::translate("Form", "connect", nullptr));
        pushButton_2->setText(QApplication::translate("Form", "start send", nullptr));
        textEdit->setHtml(QApplication::translate("Form", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Ubuntu'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:18pt; font-weight:600;\">Path_to_file</span></p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DOWNLOAD_WINDOW_2_H
