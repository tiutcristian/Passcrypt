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
    //ui->encryptButton->setCursor(Qt::PointingHandCursor);
    //ui->clipboardButton->setCursor(Qt::PointingHandCursor);
    ui->yesButton->setCursor(Qt::PointingHandCursor);
    ui->noButton->setCursor(Qt::PointingHandCursor);
    ui->xButton->setCursor(Qt::PointingHandCursor);
}

void Generate::initialState()
{
    //QRect screenGeometry = QApplication::desktop()->screenGeometry();
    //int x = (screenGeometry.width()-this->width()) / 2;
    //int y = (screenGeometry.height()-this->height()) / 2;
    //setGeometry(x, y, 350, 105);
    //ui->inputLineEdit->setText("");
    //ui->encryptedLineEdit->setText("");
    //ui->encryptedLineEdit->setDisabled(true);
}

void Generate::connectComponents()
{
    //connect(ui->encryptButton, SIGNAL( pressed()), this, SLOT( encryptPressed()) );
    //connect(ui->clipboardButton, SIGNAL( pressed()), this, SLOT( clipboardPressed()) );
    connect(ui->yesButton, SIGNAL( pressed()), this, SLOT( yesPressed()) );
    connect(ui->noButton, SIGNAL( pressed()), this, SLOT( noPressed()) );
    connect(ui->inputLineEdit, &QLineEdit::textChanged, [=]{ style()->polish(ui->inputLineEdit); });
    connect(ui->xButton, SIGNAL( pressed()), this, SLOT( xPressed()) );
}

void Generate::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setBrush(QBrush(QColor(5, 16, 32)));
    painter.setPen(QColor(52, 105, 157));
    painter.setRenderHint(QPainter::Antialiasing);

    // Change border radius
    QRect rect = this->rect();
    rect.setWidth(rect.width()-1);
    rect.setHeight(rect.height()-1);
    painter.drawRoundedRect (rect, 15, 15);

    QWidget::paintEvent(event);
}

Generate::Generate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Generate)
{
    ui->setupUi(this);
    initialState();
    buttonsStyle();
    connectComponents();
    setAttribute(Qt::WA_TranslucentBackground);
}

Generate::~Generate()
{
    delete ui;
}

void Generate::expand()
{
    auto anim = new QPropertyAnimation(this, "geometry");
    anim->setDuration(400);
    anim->setStartValue(this->geometry());
    int x = this->x();
    int y = this->y();
    anim->setEndValue(QRect(x, y, 350, 300));
    anim->start();
}

void Generate::encryptPressed()
{
    if(ui->inputLineEdit->text().length())
    {
        encryptedPassword = ui->inputLineEdit->text().toStdString();
        //ui->encryptedLineEdit->setText( ui->inputLineEdit->text() );
        expand();
    }
}

void Generate::clipboardPressed()
{
    QClipboard *clipboard = QApplication::clipboard();
    //clipboard->setText( ui->encryptedLineEdit->text() );
}

void Generate::yesPressed()
{
    save_pass_dialog2 dialog;
    dialog.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    int x = this->geometry().x();
    int y = this->geometry().y();
    dialog.setGeometry(x, y, 350, 380);
    dialog.exec();
    if(dialog.toSave())
    {
        save = true;
        passwordName = dialog.getName();
        passwordID = dialog.getID();
        passwordDescription = dialog.getDescription();
    }
    else
        save = false;
    initialState();
    this->close();
}

void Generate::noPressed()
{
    save = false;
    initialState();
    this->close();
}

void Generate::xPressed()
{
    this->close();
}
