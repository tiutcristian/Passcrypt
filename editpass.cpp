#include "editpass.h"
#include "ui_editpass.h"

EditPass::EditPass(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditPass)
{
    ui->setupUi(this);
}

EditPass::EditPass(const QString &name, const QString &id, const QString &description, const QString &password)
{
    this->name = name;
    this->id = id;
    this->description = description;
    this->password = password;
}

EditPass::~EditPass()
{
    delete ui;
}
