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
#include "save_pass_dialog2.h"
#include "constants.h"

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
    connect(ui->autoButton, SIGNAL(pressed()), this, SLOT(autoPressed()));
    connect(ui->manualButton, SIGNAL(pressed()), this, SLOT(manualPressed()));
    connect(ui->databaseButton, SIGNAL(pressed()), this, SLOT(databasePressed()));
    connect(ui->helpButton, SIGNAL(pressed()), this, SLOT(helpPressed()));
    /*
    int a = 3;
    int b = 4;
    connect(ui->pushButton, &QPushButton::clicked, [a, b](){
       std::cout << a << ' ' << b << std::endl;
    });*/
}

void MainWindow::buttonStyle()
{
    ui->homeButton->setCursor(Qt::PointingHandCursor);
    ui->getStartedButton->setCursor(Qt::PointingHandCursor);
    ui->newButton->setCursor(Qt::PointingHandCursor);
    ui->autoButton->setCursor(Qt::PointingHandCursor);
    ui->manualButton->setCursor(Qt::PointingHandCursor);
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
    for(auto &crt : db->entries)
    {
        isEmpty = false;

        auto passhlay = new QHBoxLayout;

        auto namelbl = new QLabel(QString::fromStdString(crt.title));
        auto idlbl = new QLabel(QString::fromStdString(crt.username));
        auto descriptionlbl = new QLabel(QString::fromStdString(crt.description));
        auto editButton = new QPushButton(QIcon(":/icons/icons/edit-2 (1).svg"), " edit");
        auto deleteButton = new QPushButton(QIcon(":/icons/icons/trash-2-lightblue.svg"), " delete");

        passhlay->addWidget(namelbl);
        passhlay->addWidget(idlbl);
        passhlay->addWidget(descriptionlbl);
        passhlay->addWidget(editButton);
        passhlay->addWidget(deleteButton);

        namelbl->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
        idlbl->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
        descriptionlbl->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);

        editButton->setMinimumHeight(25);
        editButton->setMaximumWidth(70);
        editButton->setCursor(Qt::PointingHandCursor);
        connect(editButton, &QPushButton::clicked, [&crt, this](){
            openEditPass(crt);
        });

        deleteButton->setMinimumHeight(25);
        deleteButton->setMaximumWidth(70);
        deleteButton->setCursor(Qt::PointingHandCursor);

        auto passwid = new QWidget;
        passwid->setLayout(passhlay);
        ui->passLayout->addWidget(passwid);
        ui->passwordsSubcontainer->setStyleSheet(PasswordsSubContainerStyleSheet);

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

void MainWindow::openEditPass(Database::Entry &entry)
{
    EditPass* editpass = new EditPass(entry);
    editpass->exec();
    if(editpass->toSave())
    {
        entry = editpass->getEntry();
        updateDatabaseUI();
        db->save();
    }
}

void MainWindow::expandDatabaseCreateToolbar()
{
    auto anim1 = new QPropertyAnimation(ui->autoButton, "geometry");
    anim1->setDuration(300);
    anim1->setStartValue(ui->autoButton->geometry());

    auto anim2 = new QPropertyAnimation(ui->manualButton, "geometry");
    anim2->setDuration(300);
    anim2->setStartValue(ui->manualButton->geometry());

    createNewToggled = 1;
    anim1->setEndValue(QRect(50, 0, 80, 40));
    anim2->setEndValue(QRect(140, 0, 80, 40));

    anim1->start();
    anim2->start();
}

void MainWindow::shrinkDatabaseCreateToolbar()
{
    auto anim1 = new QPropertyAnimation(ui->autoButton, "geometry");
    anim1->setDuration(300);
    anim1->setStartValue(ui->autoButton->geometry());

    auto anim2 = new QPropertyAnimation(ui->manualButton, "geometry");
    anim2->setDuration(300);
    anim2->setStartValue(ui->manualButton->geometry());

    createNewToggled = 0;
    anim1->setEndValue(QRect(0, 0, 40, 40));
    anim2->setEndValue(QRect(0, 0, 40, 40));

    anim1->start();
    anim2->start();
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
    if(!createNewToggled)
        expandDatabaseCreateToolbar();
    else
        shrinkDatabaseCreateToolbar();
}

void MainWindow::autoPressed()
{
    shrinkDatabaseCreateToolbar();
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

void MainWindow::manualPressed()
{
    shrinkDatabaseCreateToolbar();
}

void MainWindow::helpPressed()
{
    uncheckAllButtons(ui->leftMenuContainer);
    ui->stackedWidget->setCurrentWidget(ui->helpPage);
}
