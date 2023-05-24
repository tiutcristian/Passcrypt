#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logindialog.h"
#include "signupdialog.h"
#include "generate.h"
#include "editpass.h"
#include <fstream>
#include <iostream>
#include <QStyle>
#include <QPropertyAnimation>
#include <QClipboard>
#include <QLineEdit>
#include <QSizePolicy>
#include "save_pass_dialog.h"

void MainWindow::initialDialog()
{
    std::ifstream fin("database.txt");
    if(fin)
        ld.show();
    else
        sd.show();
    fin.close();
}

void MainWindow::connectComponents()
{
    connect(ui->homeButton, SIGNAL(pressed()), this, SLOT(homePressed()));
    connect(ui->getStartedButton, SIGNAL(pressed()), this, SLOT(getStartedPressed()));
    connect(ui->newButton, SIGNAL(pressed()), this, SLOT(createNewPressed()));
    connect(ui->databaseButton, SIGNAL(pressed()), this, SLOT(databasePressed()));
    connect(ui->helpButton, SIGNAL(pressed()), this, SLOT(helpPressed()));
    connect(ui->pushButton, SIGNAL(pressed()), this, SLOT(editPressed()));
}

void MainWindow::buttonStyle()
{
    ui->homeButton->setCursor(Qt::PointingHandCursor);
    ui->getStartedButton->setCursor(Qt::PointingHandCursor);
    ui->newButton->setCursor(Qt::PointingHandCursor);
    ui->databaseButton->setCursor(Qt::PointingHandCursor);
    ui->helpButton->setCursor(Qt::PointingHandCursor);
}

void MainWindow::initialState()
{
    uncheckAllButtons(ui->leftMenuContainer);
    ui->homeButton->setChecked(true);
    ui->stackedWidget->setCurrentWidget(ui->homePage);
}

void MainWindow::uncheckAllButtons(QObject *widget)
{
    auto button = dynamic_cast<QPushButton*>(widget);
    if(button != nullptr)
        button->setChecked(false);
    else
    {
        auto list = widget->children();
        for(auto i : list)
            uncheckAllButtons(i);
    }
}

void MainWindow::updateDatabaseUI()
{
    clearLayout(ui->passLayout);
    bool isEmpty = true;
    for(const auto &crt : db->entries)
    {
        isEmpty = false;
        auto passhlay = new QHBoxLayout;
        auto namelbl = new QLabel(QString::fromStdString(crt.name));
        auto idlbl = new QLabel(QString::fromStdString(crt.id));
        auto descriptionlbl = new QLabel(QString::fromStdString(crt.description));
        auto passLineEdit = new QLineEdit(QString::fromStdString(crt.pass));
        passLineEdit->setCursorPosition(0);
        passLineEdit->setDisabled(true);
        passLineEdit->setEchoMode(QLineEdit::Password);
        passhlay->addWidget(namelbl);
        passhlay->addWidget(idlbl);
        passhlay->addWidget(descriptionlbl);
        passhlay->addWidget(passLineEdit);
        namelbl->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
        idlbl->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
        descriptionlbl->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
        passLineEdit->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
        auto passwid = new QWidget;
        passwid->setLayout(passhlay);
        passwid->setCursor(Qt::PointingHandCursor);
        ui->passLayout->addWidget(passwid);
        ui->passwordsSubcontainer->setStyleSheet("#passwordsSubcontainer{background-color: qlineargradient("
                                                 "spread:pad, x1:0, y1:1, x2:1, y2:0, stop:0 rgba(57, 73, 94, 255), "
                                                 "stop:1 rgba(68, 87, 112, 255));border-radius: 26px;}"
                                                 "#passwordsSubcontainer QLabel{color: rgb(153, 234, 255);font: 10pt \"Segoe UI\";}"
                                                 "#passwordsSubcontainer QWidget{border-radius: 17px;}"
                                                 "#passwordsSubcontainer QWidget:hover{background-color: rgb(71, 91, 117);}"
                                                 "#passwordsSubcontainer QLineEdit{color: rgb(153, 234, 255);}");
    }
    if(isEmpty)
    {
        ui->passwordsSubcontainer->setStyleSheet("background: transparent;");
        auto emptyhlay = new QHBoxLayout;
        emptyhlay->addStretch();
        auto emptylbl = new QLabel("No saved passwords");
        emptylbl->setStyleSheet("font: 12pt \"Segoe UI\"; color: #34699D;");
        emptyhlay->addWidget(emptylbl);
        emptyhlay->addStretch();
        auto emptywid = new QWidget;
        emptywid->setLayout(emptyhlay);
        ui->passLayout->addWidget(emptywid);
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      ld(this),
      sd(this)
{
    ui->setupUi(this);
    initialDialog();
    connectComponents();
    buttonStyle();
    initialState();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clearLayout(QLayout *layout)
{
    QLayoutItem* child;
    while(layout->count()!= 0)
    {
        child = layout->takeAt(0);
        if (child->layout() != 0)
            clearLayout(child->layout());
        else if(child->widget() != 0)
            delete child->widget();
        delete child;
    }
}

void MainWindow::setDatabase(Database *db)
{
    this->db = db;
    updateDatabaseUI();
}

void MainWindow::homePressed()
{
    uncheckAllButtons(ui->leftMenuContainer);
    ui->stackedWidget->setCurrentWidget(ui->homePage);
}

void MainWindow::getStartedPressed()
{
    uncheckAllButtons(ui->leftMenuContainer);
    ui->databaseButton->setChecked(true);
    ui->stackedWidget->setCurrentWidget(ui->databasePage);
}

void MainWindow::databasePressed()
{
    uncheckAllButtons(ui->leftMenuContainer);
    ui->stackedWidget->setCurrentWidget(ui->databasePage);
}

void MainWindow::createNewPressed()
{
    Generate gen(this);
    gen.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    gen.exec();
    if(gen.toSave())
    {
        auto name = gen.getName();
        auto ID = gen.getID();
        auto desc = gen.getDescription();
        auto pass = gen.getEncryptedPassword();
        std::cout << name << ' ' << ID << ' ' << desc << ' ' << pass << std::endl;

        db->add(Database::Entry(name, ID, desc, pass));
        updateDatabaseUI();
    }
}

void MainWindow::helpPressed()
{
    uncheckAllButtons(ui->leftMenuContainer);
    ui->stackedWidget->setCurrentWidget(ui->helpPage);
}

void MainWindow::editPressed()
{
    EditPass* editpass = new EditPass("name", "id", "desc", "pass");
    //EditPass* editpass = new EditPass();
    editpass->show();
}
