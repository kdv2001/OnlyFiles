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
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QStackedWidget *stack;
    QWidget *page;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QLineEdit *file_path;
    QSlider *horizontalSlider;
    QPushButton *pushButton_3;
    QLabel *label;
    QLineEdit *line_file_number;
    QLineEdit *line_pin;
    QPushButton *clear_button;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QString::fromUtf8("Form"));
        Form->setEnabled(true);
        Form->resize(640, 460);
        QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Form->sizePolicy().hasHeightForWidth());
        Form->setSizePolicy(sizePolicy);
        Form->setMaximumSize(QSize(640, 460));
        Form->setAutoFillBackground(false);
        stack = new QStackedWidget(Form);
        stack->setObjectName(QString::fromUtf8("stack"));
        stack->setGeometry(QRect(0, 0, 640, 460));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(stack->sizePolicy().hasHeightForWidth());
        stack->setSizePolicy(sizePolicy1);
        stack->setAutoFillBackground(true);
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        pushButton = new QPushButton(page);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(20, 350, 291, 71));
        QFont font;
        font.setPointSize(18);
        pushButton->setFont(font);
        pushButton->setMouseTracking(true);
        QIcon icon;
        icon.addFile(QString::fromUtf8("../../Downloads/down-arrow.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton->setIcon(icon);
        pushButton->setIconSize(QSize(48, 48));
        pushButton_2 = new QPushButton(page);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(330, 350, 291, 71));
        pushButton_2->setFont(font);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8("../image/up-arrow.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_2->setIcon(icon1);
        pushButton_2->setIconSize(QSize(48, 48));
        file_path = new QLineEdit(page);
        file_path->setObjectName(QString::fromUtf8("file_path"));
        file_path->setGeometry(QRect(66, 70, 511, 51));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Ubuntu"));
        font1.setPointSize(17);
        file_path->setFont(font1);
        horizontalSlider = new QSlider(page);
        horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
        horizontalSlider->setGeometry(QRect(20, 160, 501, 51));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(horizontalSlider->sizePolicy().hasHeightForWidth());
        horizontalSlider->setSizePolicy(sizePolicy2);
        horizontalSlider->setCursor(QCursor(Qt::ArrowCursor));
        horizontalSlider->setMaximum(100);
        horizontalSlider->setOrientation(Qt::Horizontal);
        pushButton_3 = new QPushButton(page);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(6, 70, 61, 51));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8("../../Downloads/folder.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_3->setIcon(icon2);
        pushButton_3->setIconSize(QSize(32, 32));
        label = new QLabel(page);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(550, 150, 67, 61));
        line_file_number = new QLineEdit(page);
        line_file_number->setObjectName(QString::fromUtf8("line_file_number"));
        line_file_number->setGeometry(QRect(20, 250, 291, 41));
        QFont font2;
        font2.setPointSize(14);
        line_file_number->setFont(font2);
        line_pin = new QLineEdit(page);
        line_pin->setObjectName(QString::fromUtf8("line_pin"));
        line_pin->setGeometry(QRect(20, 300, 291, 41));
        line_pin->setFont(font2);
        clear_button = new QPushButton(page);
        clear_button->setObjectName(QString::fromUtf8("clear_button"));
        clear_button->setGeometry(QRect(573, 70, 61, 51));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8("../image/right-arroww.png"), QSize(), QIcon::Normal, QIcon::Off);
        clear_button->setIcon(icon3);
        clear_button->setIconSize(QSize(32, 32));
        stack->addWidget(page);

        retranslateUi(Form);
        QObject::connect(clear_button, SIGNAL(clicked()), file_path, SLOT(clear()));

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "OnlyFiles", nullptr));
        pushButton->setText(QApplication::translate("Form", " Download", nullptr));
        pushButton_2->setText(QApplication::translate("Form", " Start send", nullptr));
        file_path->setPlaceholderText(QApplication::translate("Form", "                                        Path to file", nullptr));
        pushButton_3->setText(QString());
        label->setText(QApplication::translate("Form", "<html><head/><body><p align=\"center\">comming</p><p align=\"center\">soon</p></body></html>", nullptr));
        line_file_number->setInputMask(QString());
        line_file_number->setText(QString());
        line_file_number->setPlaceholderText(QApplication::translate("Form", "                    File number", nullptr));
        line_pin->setPlaceholderText(QApplication::translate("Form", "                             Pin", nullptr));
        clear_button->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DOWNLOAD_WINDOW_2_H
