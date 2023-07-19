#include "editpass.h"
#include "ui_editpass.h"
#include <QStyle>
#include <iostream>
#include <QMessageBox>

EditPass::EditPass(int index, Database *db) :
    ui(new Ui::EditPass),
    entry(db->entries[index]),
    index(index),
    db(db)
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
    ui->editButton->setCursor(Qt::PointingHandCursor);
    ui->saveButton->setCursor(Qt::PointingHandCursor);
    ui->cancelButton->setCursor(Qt::PointingHandCursor);
}

void EditPass::connectComponents()
{
    // text changed
    connect(ui->titleLineEdit, &QLineEdit::textChanged, [=]{ style()->polish(ui->titleLineEdit); });
    connect(ui->idLineEdit, &QLineEdit::textChanged, [=]{ style()->polish(ui->idLineEdit); });
    connect(ui->passwordLineEdit, &QLineEdit::textChanged, [=]{ passwordChanged(); });
    connect(ui->descriptionTextEdit, SIGNAL(textChanged()), this, SLOT(descriptionChanged()));

    // button clicked
    connect(ui->editButton, SIGNAL(clicked()), this, SLOT(editClicked()));
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(saveClicked()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancelClicked()));

    // timer timeout
    connect(&timer, SIGNAL(timeout()), this, SLOT(editTimedOut()));
}

void EditPass::initialState()
{
    // initialize title line edit
    ui->titleLineEdit->setText(QString::fromStdString(entry.title));
    ui->titleLineEdit->setCursorPosition(0);

    // initialize id line edit
    ui->idLineEdit->setText(QString::fromStdString(entry.username));
    ui->idLineEdit->setCursorPosition(0);

    // initialize description line edit
    ui->descriptionTextEdit->setText(QString::fromStdString(entry.description));

    // initialize password line edit
    ui->passwordLineEdit->setText(QString::fromStdString(entry.pass));
    ui->passwordLineEdit->setCursorPosition(0);

    // general initializations
    setFocus(Qt::NoFocusReason);
}

void EditPass::passwordChanged()
{
    if(timer.isActive())
        timer.stop();
    timer.start(5000);
}

void EditPass::descriptionChanged()
{
    QString text = ui->descriptionTextEdit->toPlainText();
    if(text == "")
        ui->descriptionTextEdit->setStyleSheet("color: rgba(153, 234, 255, 70);");
    else
        ui->descriptionTextEdit->setStyleSheet("color: rgba(153, 234, 255, 255);");
}

void EditPass::editClicked()
{
    if(!editButtonToggled)
    {
        editButtonToggled = true;
        ui->editButton->setText("Hide");
        ui->editButton->setIcon(QIcon(":/icons/icons/eye-off-lightblue.svg"));
        ui->passwordLineEdit->setEchoMode(QLineEdit::Normal);
        ui->passwordLineEdit->setEnabled(true);
        if(timer.isActive())
            timer.stop();
        timer.start(5000);
    }
    else
    {
        editButtonToggled = false;
        ui->editButton->setText("Edit");
        ui->editButton->setIcon(QIcon(":/icons/icons/edit-2 (1).svg"));
        ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
        ui->passwordLineEdit->setDisabled(true);
    }
}

void EditPass::saveClicked()
{
    if(db->availableTitle(ui->titleLineEdit->text().toStdString(), db->entries, index))
    {
        entry.title = ui->titleLineEdit->text().toStdString();
        entry.username = ui->idLineEdit->text().toStdString();
        entry.description = ui->descriptionTextEdit->toPlainText().toStdString();
        entry.pass = ui->passwordLineEdit->text().toStdString();
        save = true;
        EditPass::close();
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

void EditPass::cancelClicked()
{
    EditPass::close();
}

void EditPass::editTimedOut()
{
    editButtonToggled = false;
    ui->editButton->setText("Edit");
    ui->editButton->setIcon(QIcon(":/icons/icons/edit-2 (1).svg"));
    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
    ui->passwordLineEdit->setDisabled(true);
}
