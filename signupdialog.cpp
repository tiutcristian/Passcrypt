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
    auto cursor = new QCursor;
    cursor->setShape(Qt::PointingHandCursor);
    ui->confirmButton->setCursor(*cursor);
}

void signupDialog::connectComponents()
{
    connect(ui->confirmButton, SIGNAL( clicked() ), this, SLOT( confirmClicked() ) );
    connect(ui->enterLineEdit, &QLineEdit::textChanged, [=]{ style()->polish(ui->enterLineEdit); ui->notMatchLabel->hide(); });
    connect(ui->reenterLineEdit, &QLineEdit::textChanged, [=]{ style()->polish(ui->reenterLineEdit); ui->notMatchLabel->hide(); });
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
