#include "generate.h"
#include "ui_generate.h"
#include <fstream>
#include <iostream>
#include <QPropertyAnimation>
#include <QClipboard>
#include "save_pass_dialog2.h"
#include <QString>
#include <QStyle>
#include <QDesktopWidget>
#include <QPainter>

void Generate::buttonsStyle()
{
    ui->yesButton->setCursor(Qt::PointingHandCursor);
    ui->noButton->setCursor(Qt::PointingHandCursor);
}

void Generate::initialState(const bool &autoGenerate)
{
    if (autoGenerate)
        ui->passwordLineEdit->setText("abc");
    else
        ui->passwordLineEdit->setPlaceholderText("Enter password...");
}

void Generate::connectComponents()
{
    connect(ui->titleLineEdit, &QLineEdit::textChanged, [=]{ style()->polish(ui->titleLineEdit); });
    connect(ui->idLineEdit, &QLineEdit::textChanged, [=]{ style()->polish(ui->idLineEdit); });
    connect(ui->passwordLineEdit, &QLineEdit::textChanged, [=]{ style()->polish(ui->passwordLineEdit); });
    connect(ui->descriptionTextEdit, SIGNAL( textChanged() ), this, SLOT( descriptionChanged() ));
    connect(ui->yesButton, SIGNAL( pressed()), this, SLOT( yesPressed()) );
    connect(ui->noButton, SIGNAL( pressed()), this, SLOT( noPressed()) );
}

Generate::Generate(const bool &autoGenerate) :
    ui(new Ui::Generate)
{
    ui->setupUi(this);
    initialState(autoGenerate);
    buttonsStyle();
    connectComponents();
}

Generate::~Generate()
{
    delete ui;
}

void Generate::descriptionChanged()
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

void Generate::yesPressed()
{
    save = true;
    title = ui->titleLineEdit->text().toStdString();
    id = ui->idLineEdit->text().toStdString();
    password = ui->passwordLineEdit->text().toStdString();
    description = ui->descriptionTextEdit->toPlainText().toStdString();
    this->close();
}

void Generate::noPressed()
{
    save = false;
    this->close();
}
