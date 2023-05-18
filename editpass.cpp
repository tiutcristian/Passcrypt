#include "editpass.h"
#include "ui_editpass.h"

EditPass::EditPass(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditPass)
{
    ui->setupUi(this);
}

EditPass::~EditPass()
{
    delete ui;
}
