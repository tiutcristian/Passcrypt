#include "signupdialog.h"
#include "ui_signupdialog.h"
#include "mainwindow.h"
#include "database.h"
#include <string>
#include <QStyle>
#include <QMessageBox>

signupDialog::signupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::signupDialog)
{
    ui->setupUi(this);
    buttonStyle();
    connectComponents();
    ui->enterLineEdit->setMaxLength(100);
    ui->notMatchLabel->hide();
}

signupDialog::~signupDialog()
{
    delete ui;
}

void signupDialog::buttonStyle()
{
    ui->confirmButton->setCursor(Qt::PointingHandCursor);
}

void signupDialog::connectComponents()
{
    // text changed
    connect(ui->enterLineEdit, &QLineEdit::textChanged, [=]{ style()->polish(ui->enterLineEdit); ui->notMatchLabel->hide(); });
    connect(ui->reenterLineEdit, &QLineEdit::textChanged, [=]{ style()->polish(ui->reenterLineEdit); ui->notMatchLabel->hide(); });

    // button clicked
    connect(ui->confirmButton, SIGNAL( clicked() ), this, SLOT( confirmClicked() ) );
}

void signupDialog::confirmClicked()
{
    if(ui->enterLineEdit->text() == ui->reenterLineEdit->text())
    {
        auto mainwindow = ((MainWindow*) parent());
        Database *db = new Database(true, ui->enterLineEdit->text().toStdString());
        mainwindow->setDatabase(db);
        mainwindow->show();
        this->close();
    }
    else
    {
        ui->notMatchLabel->show();
    }
}
