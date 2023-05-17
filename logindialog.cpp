#include "logindialog.h"
#include "ui_logindialog.h"
#include "mainwindow.h"
#include <fstream>
#include <string>
#include <QStyle>

loginDialog::loginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::loginDialog)
{
    ui->setupUi(this);
    buttonStyle();
    connectComponents();
    ui->lineEdit->setMaxLength(100);
}

loginDialog::~loginDialog()
{
    delete ui;
}

void loginDialog::buttonStyle()
{
    auto cursor = new QCursor;
    cursor->setShape(Qt::PointingHandCursor);
    ui->confirmButton->setCursor(*cursor);
}

void loginDialog::connectComponents()
{
    connect( ui->lineEdit, &QLineEdit::textChanged, [=]{ style()->polish(ui->lineEdit); });
    connect( ui->confirmButton, SIGNAL( pressed() ), this, SLOT( confirmPressed() ) );
}

void loginDialog::confirmPressed()
{
    try {
        auto db = new Database(false, ui->lineEdit->text().toStdString());
        auto mainwindow = ((MainWindow*) parent());
        mainwindow->setDatabase(db);
        mainwindow->show();
        this->close();
    }  catch (Database::BadPasswordException) {
        ;//messagebox
    }
}
