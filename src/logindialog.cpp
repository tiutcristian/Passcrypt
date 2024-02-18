#include "logindialog.h"
#include "ui_logindialog.h"
#include "mainwindow.h"
// #include <fstream>
#include <iostream>
// #include <string>
#include <QStyle>
#include <QMessageBox>

loginDialog::loginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::loginDialog)
{
    ui->setupUi(this);
    buttonStyle();
    connectComponents();
    ui->wrongLabel->hide();
}

loginDialog::~loginDialog()
{
    delete ui;
}

void loginDialog::buttonStyle()
{
    ui->confirmButton->setCursor(Qt::PointingHandCursor);
}

void loginDialog::connectComponents()
{
    // text changed
    connect( ui->lineEdit, &QLineEdit::textChanged, [=]{ style()->polish(ui->lineEdit); ui->wrongLabel->hide(); });

    // button clicked
    connect( ui->confirmButton, SIGNAL( clicked() ), this, SLOT( confirmClicked() ) );
    connect( this, SIGNAL( finished(int) ), this, SLOT( window_closed()) );
}

void loginDialog::confirmClicked()
{
    try
    {
        auto db = new Database(false, ui->lineEdit->text().toStdString());
        auto mainwindow = ((MainWindow*) parent());
        mainwindow->setDatabase(db);
        mainwindow->show();
        confirmedClicked = true;
        this->close();
    }
    catch (Database::BadPasswordException)
    {
        ui->wrongLabel->show();
    }
}

void loginDialog::window_closed()
{
    if(!confirmedClicked)
        QApplication::quit();
}
