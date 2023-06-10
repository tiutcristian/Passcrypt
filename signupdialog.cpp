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
    ui->reenterLineEdit->setMaxLength(100);
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
    connect(ui->enterLineEdit, &QLineEdit::textChanged, [=]{ style()->polish(ui->enterLineEdit); });
    connect(ui->reenterLineEdit, &QLineEdit::textChanged, [=]{ style()->polish(ui->reenterLineEdit); });
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
        QMessageBox box(QMessageBox::Icon::Critical, "Error", "Passwords do not match", QMessageBox::Ok, this);
        box.exec();
    }
}
