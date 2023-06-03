#include "mainwindow.h"
#include <QApplication>
#include <QString>
#include "encrypt.h"
#include <cryptopp/default.h>
#include <iostream>
#include <QPalette>
#include <QStyleFactory>
#include "sodium.h"

/*
int foo() {
    unsigned char myString[32];
    uint32_t myInt;

    randombytes_buf(myString, 32);
    // myString will be an array of 32 random bytes, not null-terminated
    myInt = randombytes_uniform(10);
    // myInt will be a random number between 0 and 9
}
*/

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

    //foo();
    /*
    char_options co;
    co.lowercase = 0;
    co.uppercase = 0;
    co.numbers = 0;
    co.symbols = 0;
    std::cout << generatePassword(20, co);
    return 0;
    */
}
