#include "editpass.h"
#include "ui_editpass.h"
#include <QStyle>
#include <iostream>
/*
EditPass::EditPass(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditPass)
{
    ui->setupUi(this);
    buttonStyle();
    connectComponents();
}
*/
EditPass::EditPass(const QString &name, const QString &id, const QString &description, const QString &password) :
    ui(new Ui::EditPass)
{
    ui->setupUi(this);
    this->name = name;
    this->id = id;
    this->description = description;
    this->password = password;
    buttonStyle();
    connectComponents();
    initialState();
}

EditPass::~EditPass()
{
    delete ui;
}

void EditPass::buttonStyle()
{
    auto cursor = new QCursor;
    cursor->setShape(Qt::PointingHandCursor);
    ui->editButton->setCursor(*cursor);
    ui->saveButton->setCursor(*cursor);
    ui->deleteButton->setCursor(*cursor);
}

void EditPass::connectComponents()
{
    connect(ui->nameLineEdit, &QLineEdit::textChanged, [=]{ style()->polish(ui->nameLineEdit); });
    connect(ui->idLineEdit, &QLineEdit::textChanged, [=]{ style()->polish(ui->idLineEdit); });
    connect(ui->descriptionTextEdit, SIGNAL(textChanged()), this, SLOT(descriptionChanged()));
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(savePressed()));
}

void EditPass::initialState()
{
    ui->nameLineEdit->setText(name);
    ui->idLineEdit->setText(id);
    ui->descriptionTextEdit->setText(description);
    ui->passwordLineEdit->setText(password);
    setFocus(Qt::NoFocusReason);
}

void EditPass::descriptionChanged()
{
    QString text = ui->descriptionTextEdit->toPlainText();
    if(text == "")
        ui->descriptionTextEdit->setStyleSheet("color: rgba(153, 234, 255, 70);");
    else
        ui->descriptionTextEdit->setStyleSheet("color: rgba(153, 234, 255, 255);");
}

void EditPass::savePressed()
{
    name = ui->nameLineEdit->text();
    id = ui->idLineEdit->text();
    description = ui->descriptionTextEdit->toPlainText();
    password = ui->passwordLineEdit->text();
    save = true;
    EditPass::close();
}
