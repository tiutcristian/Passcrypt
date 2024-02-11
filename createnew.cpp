#include "createnew.h"
#include "ui_createnew.h"
#include <fstream>
#include <iostream>
#include <QPropertyAnimation>
#include <QClipboard>
#include <QString>
#include <QStyle>
#include <QPainter>
#include "encrypt.h"
#include <QMessageBox>
#include "constants.h"
#include <cstdlib>

void CreateNew::buttonsStyle()
{
    ui->advancedButton->setCursor(Qt::PointingHandCursor);
    ui->upperButton->setCursor(Qt::PointingHandCursor);
    ui->lowerButton->setCursor(Qt::PointingHandCursor);
    ui->numbersButton->setCursor(Qt::PointingHandCursor);
    ui->symbolsButton->setCursor(Qt::PointingHandCursor);
    ui->regenerateButton->setCursor(Qt::PointingHandCursor);
    ui->shrinkButton->setCursor(Qt::PointingHandCursor);
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
        ui->passwordLineEdit->setDisabled(1);
    }
    else
    {
        ui->passwordLineEdit->setPlaceholderText("Enter password...");
        ui->advancedButton->hide();
    }
}

void CreateNew::connectComponents()
{
    // text changed
    connect(ui->titleLineEdit, &QLineEdit::textChanged, [=]{ style()->polish(ui->titleLineEdit); });
    connect(ui->idLineEdit, &QLineEdit::textChanged, [=]{ style()->polish(ui->idLineEdit); });
    connect(ui->passwordLineEdit, &QLineEdit::textChanged, [=]{ style()->polish(ui->passwordLineEdit); });
    connect(ui->descriptionTextEdit, SIGNAL(textChanged()), this, SLOT(descriptionChanged()));

    // button clicked
    connect(ui->advancedButton, SIGNAL(clicked()), this, SLOT(advancedClicked()));
    connect(ui->upperButton, &QPushButton::clicked, [=](){ charOptionClicked(upperToggled, ui->upperButton); });
    connect(ui->lowerButton, &QPushButton::clicked, [=](){ charOptionClicked(lowerToggled, ui->lowerButton); });
    connect(ui->numbersButton, &QPushButton::clicked, [=](){ charOptionClicked(numbersToggled, ui->numbersButton); });
    connect(ui->symbolsButton, &QPushButton::clicked, [=](){ charOptionClicked(symbolsToggled, ui->symbolsButton); });
    connect(ui->regenerateButton, SIGNAL(clicked()), this, SLOT(regenerateClicked()));
    connect(ui->shrinkButton, &QPushButton::clicked, [=](){ shrinkAdvancedMenu(); });
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(saveClicked()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancelClicked()));
}

void CreateNew::expandAdvancedMenu()
{
    advancedToggled = 1;
    auto anim = new QPropertyAnimation(ui->advancedFrame, "geometry");
    anim->setDuration(300);
    anim->setStartValue(ui->advancedFrame->geometry());
    anim->setEndValue(QRect(10, 230, 310, 200));
    anim->start();
}

void CreateNew::shrinkAdvancedMenu()
{
    advancedToggled = 0;
    auto anim = new QPropertyAnimation(ui->advancedFrame, "geometry");
    anim->setDuration(300);
    anim->setStartValue(ui->advancedFrame->geometry());
    anim->setEndValue(QRect(10, 230, 310, 0));
    anim->start();
}

void CreateNew::charOptionClicked(bool &toggled, QWidget *widget)
{
    if(toggled)
    {
        toggled = 0;
        widget->setStyleSheet(AdvancedFrameButtonDisabledStylesheet);
    }
    else
    {
        toggled = 1;
        widget->setStyleSheet("");
    }
}

CreateNew::CreateNew(bool autoGenerate, Database *db) :
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

void CreateNew::advancedClicked()
{
    if(!advancedToggled)
        expandAdvancedMenu();
    else
        shrinkAdvancedMenu();
}

void CreateNew::regenerateClicked()
{
    char_options co;
    co.lowercase = lowerToggled;
    co.uppercase = upperToggled;
    co.numbers = numbersToggled;
    co.symbols = symbolsToggled;
    int length = atoi( ui->lengthLineEdit->text().toStdString().c_str() );
    std::string newPass = generatePassword(length, co);
    ui->passwordLineEdit->setText(QString::fromStdString(newPass));
    ui->passwordLineEdit->setCursorPosition(0);
}

void CreateNew::saveClicked()
{
    if(db->availableTitle(ui->titleLineEdit->text().toStdString(), db->entries, -1))
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
