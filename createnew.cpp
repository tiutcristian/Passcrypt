#include "createnew.h"
#include "ui_createnew.h"
#include <fstream>
#include <iostream>
#include <QPropertyAnimation>
#include <QClipboard>
#include <QString>
#include <QStyle>
#include <QDesktopWidget>
#include <QPainter>
#include "encrypt.h"
#include <QMessageBox>

void CreateNew::buttonsStyle()
{
    ui->saveButton->setCursor(Qt::PointingHandCursor);
    ui->cancelButton->setCursor(Qt::PointingHandCursor);
}

void CreateNew::initialState(const bool &autoGenerate)
{
    if (autoGenerate)
    {
        char_options co;
        co.lowercase = true;
        co.uppercase = true;
        co.numbers = true;
        co.symbols = true;
        ui->passwordLineEdit->setText(QString::fromStdString( generatePassword(20, co) ));
    }
    else
        ui->passwordLineEdit->setPlaceholderText("Enter password...");
}

void CreateNew::connectComponents()
{
    connect(ui->titleLineEdit, &QLineEdit::textChanged, [=]{ style()->polish(ui->titleLineEdit); });
    connect(ui->idLineEdit, &QLineEdit::textChanged, [=]{ style()->polish(ui->idLineEdit); });
    connect(ui->passwordLineEdit, &QLineEdit::textChanged, [=]{ style()->polish(ui->passwordLineEdit); });
    connect(ui->descriptionTextEdit, SIGNAL( textChanged() ), this, SLOT( descriptionChanged() ));
    connect(ui->saveButton, SIGNAL( clicked()), this, SLOT( saveClicked()) );
    connect(ui->cancelButton, SIGNAL( clicked()), this, SLOT( cancelClicked()) );
}

CreateNew::CreateNew(const bool &autoGenerate, Database *db) :
    ui(new Ui::CreateNew),
    db(db)
{
    ui->setupUi(this);
    initialState(autoGenerate);
    buttonsStyle();
    connectComponents();
}

CreateNew::~CreateNew()
{
    delete ui;
}

void CreateNew::descriptionChanged()
{
    int descriptionLength = 200;
    QString text = ui->descriptionTextEdit->toPlainText();
    bool todo = false;
    if(text.length() > descriptionLength)
    {
        int diff = text.length() - descriptionLength;
        text.chop(diff);
        todo = true;
    }
    auto aux = text;
    text.replace("\t", "");
    text.replace("\n", "");
    text.replace("\v", "");
    text.replace("\r", "");
    text.replace("\b", "");
    text.replace("\0", "");
    text.replace("\f", "");
    text.replace("\a", "");
    if(aux.compare(text))
        todo = true;
    if(todo)
    {
        ui->descriptionTextEdit->setText(text);
        QTextCursor cursor(ui->descriptionTextEdit->textCursor());
        cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
        ui->descriptionTextEdit->setTextCursor(cursor);
    }
    if(text == "")
        ui->descriptionTextEdit->setStyleSheet("color: rgba(153, 234, 255, 70);");
    else
        ui->descriptionTextEdit->setStyleSheet("color: rgba(153, 234, 255, 255);");
}

void CreateNew::saveClicked()
{
    if(db->availableTitle(ui->titleLineEdit->text().toStdString(), db->entries))
    {
        save = true;
        title = ui->titleLineEdit->text().toStdString();
        id = ui->idLineEdit->text().toStdString();
        password = ui->passwordLineEdit->text().toStdString();
        description = ui->descriptionTextEdit->toPlainText().toStdString();
        this->close();
    }
    else if(ui->titleLineEdit->text().toStdString().empty())
    {
        QMessageBox box(QMessageBox::Icon::Critical, "No title", "No title provided", QMessageBox::Ok, this);
        box.exec();
    }
    else
    {
        QMessageBox box(QMessageBox::Icon::Critical, "Conflict", "Title already used", QMessageBox::Ok, this);
        box.exec();
    }
}

void CreateNew::cancelClicked()
{
    save = false;
    this->close();
}
