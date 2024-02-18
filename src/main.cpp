#include "mainwindow.h"
#include "encrypt.h"
#include "sodium.h"
#include <QApplication>
#include <QPalette>
#include <QStyleFactory>

void setPalette(QApplication &a)
{
    a.setStyle(QStyleFactory::create("Fusion"));
    QPalette p = qApp->palette();

    p.setColor(QPalette::Window, QColor(15,16,18));
    p.setColor(QPalette::Button, QColor(33,36,51));
    p.setColor(QPalette::ButtonText, QColor(107,146,215));
    p.setColor(QPalette::WindowText, QColor(177,178,183));

    a.setPalette(p);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    setPalette(a);
    MainWindow mainw;
    return a.exec();
}
