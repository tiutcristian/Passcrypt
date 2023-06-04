#include "editpass.h"
#include "ui_editpass.h"
#include <QStyle>
#include <iostream>

EditPass::EditPass(const Database::Entry &entry) :
    ui(new Ui::EditPass),
    entry(entry)
{
    ui->setupUi(this);
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
    ui->cancelButton->setCursor(*cursor);
}

void EditPass::connectComponents()
{
    connect(ui->titleLineEdit, &QLineEdit::textChanged, [=]{ style()->polish(ui->titleLineEdit); });
    connect(ui->idLineEdit, &QLineEdit::textChanged, [=]{ style()->polish(ui->idLineEdit); });
    connect(ui->descriptionTextEdit, SIGNAL(textChanged()), this, SLOT(descriptionChanged()));
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(saveClicked()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancelClicked()));
}

void EditPass::initialState()
{
    ui->titleLineEdit->setText(QString::fromStdString(entry.title));
    ui->idLineEdit->setText(QString::fromStdString(entry.username));
    ui->descriptionTextEdit->setText(QString::fromStdString(entry.description));
    ui->passwordLineEdit->setText(QString::fromStdString(entry.pass));
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

void EditPass::saveClicked()
{
    entry.title = ui->titleLineEdit->text().toStdString();
    entry.username = ui->idLineEdit->text().toStdString();
    entry.description = ui->descriptionTextEdit->toPlainText().toStdString();
    entry.pass = ui->passwordLineEdit->text().toStdString();
    save = true;
    EditPass::close();
}

void EditPass::cancelClicked()
{
    EditPass::close();
}
