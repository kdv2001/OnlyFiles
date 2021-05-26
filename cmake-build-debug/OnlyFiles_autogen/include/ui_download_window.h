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
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QProgressBar *progressBar;

    void setupUi(QWidget *download_window)
    {
        if (download_window->objectName().isEmpty())
            download_window->setObjectName(QString::fromUtf8("download_window"));
        download_window->resize(640, 460);
        QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(download_window->sizePolicy().hasHeightForWidth());
        download_window->setSizePolicy(sizePolicy);
        download_window->setMaximumSize(QSize(640, 460));
        download_window->setCursor(QCursor(Qt::WaitCursor));
        cancel_button = new QPushButton(download_window);
        cancel_button->setObjectName(QString::fromUtf8("cancel_button"));
        cancel_button->setGeometry(QRect(310, 330, 301, 71));
        QFont font;
        font.setPointSize(18);
        cancel_button->setFont(font);
        verticalLayoutWidget = new QWidget(download_window);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(20, 80, 591, 141));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        progressBar = new QProgressBar(verticalLayoutWidget);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setEnabled(true);
        progressBar->setMinimumSize(QSize(0, 46));
        progressBar->setCursor(QCursor(Qt::WaitCursor));

        verticalLayout->addWidget(progressBar);

        verticalLayoutWidget->raise();
        cancel_button->raise();

        retranslateUi(download_window);

        QMetaObject::connectSlotsByName(download_window);
    } // setupUi

    void retranslateUi(QWidget *download_window)
    {
        download_window->setWindowTitle(QApplication::translate("download_window", "OnlyFiles", nullptr));
        cancel_button->setText(QApplication::translate("download_window", "cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class download_window: public Ui_download_window {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DOWNLOAD_WINDOW_H
