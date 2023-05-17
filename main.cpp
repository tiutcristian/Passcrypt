#include "mainwindow.h"
#include <QApplication>
#include <QString>
#include "encrypt.h"
#include <cryptopp/default.h>
#include <iostream>
#include <QPalette>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    /*
    try{
        string e = encrypt("abc", "123");
        string d = decrypt(e, "123");
    }
    catch (CryptoPP::KeyBadErr)
    {
        std::cout << "e" << std::endl;
    }
    */
    QApplication a(argc, argv);
    MainWindow mainw;
    return a.exec();
}
