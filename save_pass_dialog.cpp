#include "save_pass_dialog.h"
#include "ui_save_pass_dialog.h"
#include <QStyle>
#include <QPainter>

save_pass_dialog::save_pass_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::save_pass_dialog)
{
    ui->setupUi(this);
    buttonsStyle();
    connectComponents();
    setAttribute(Qt::WA_TranslucentBackground);
}

save_pass_dialog::~save_pass_dialog()
{
    delete ui;
}

void save_pass_dialog::connectComponents()
{
    connect(ui->saveButton, SIGNAL( pressed() ), this, SLOT( savePressed() ));
    connect(ui->cancelButton, SIGNAL( pressed() ), this, SLOT( cancelPressed() ));
    connect(ui->descriptionTextEdit, SIGNAL( textChanged() ), this, SLOT( descriptionChanged() ));
    connect(ui->lineEdit, &QLineEdit::textChanged, [=]{ style()->polish(ui->lineEdit); });
    connect(ui->lineEdit_2, &QLineEdit::textChanged, [=]{ style()->polish(ui->lineEdit_2); });
}

void save_pass_dialog::buttonsStyle()
{
    auto cursor = new QCursor;
    cursor->setShape(Qt::PointingHandCursor);
    ui->saveButton->setCursor(*cursor);
    ui->cancelButton->setCursor(*cursor);
}

void save_pass_dialog::paintEvent(QPaintEvent *event)
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
void save_pass_dialog::savePressed()
{
    name = ui->lineEdit->text().toStdString();
    ID = ui->lineEdit_2->text().toStdString();
    description = ui->descriptionTextEdit->toPlainText().toStdString();
    save = true;
    save_pass_dialog::close();
}

void save_pass_dialog::cancelPressed()
{
    save = false;
    save_pass_dialog::close();
}

void save_pass_dialog::descriptionChanged()
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
