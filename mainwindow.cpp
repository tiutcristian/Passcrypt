#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logindialog.h"
#include "signupdialog.h"
#include "createnew.h"
#include "editpass.h"
#include <fstream>
#include <iostream>
#include <QStyle>
#include <QPropertyAnimation>
#include <QLineEdit>
#include <QSizePolicy>
#include "constants.h"
#include <QMessageBox>
#include "clipboard.h"

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
    connect(ui->databaseButton, SIGNAL(pressed()), this, SLOT(databasePressed()));
    connect(ui->helpButton, SIGNAL(pressed()), this, SLOT(helpPressed()));
    connect(ui->getStartedButton, SIGNAL(clicked()), this, SLOT(getStartedClicked()));
    connect(ui->newButton, SIGNAL(clicked()), this, SLOT(createNewClicked()));
    connect(ui->autoButton, SIGNAL(clicked()), this, SLOT(autoClicked()));
    connect(ui->manualButton, SIGNAL(clicked()), this, SLOT(manualClicked()));
    connect(&timer, SIGNAL(timeout()), this, SLOT(clipboardTimedOut()));
}

void MainWindow::buttonStyle()
{
    ui->homeButton->setCursor(Qt::PointingHandCursor);
    ui->databaseButton->setCursor(Qt::PointingHandCursor);
    ui->helpButton->setCursor(Qt::PointingHandCursor);
    ui->getStartedButton->setCursor(Qt::PointingHandCursor);
    ui->newButton->setCursor(Qt::PointingHandCursor);
    ui->autoButton->setCursor(Qt::PointingHandCursor);
    ui->manualButton->setCursor(Qt::PointingHandCursor);
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
    else if(widget != nullptr)
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
    for(int index = 0; index < (int)db->entries.size(); index++)
    {
        auto &crt = db->entries[index];
        isEmpty = false;
        auto passhlay = new QHBoxLayout;
        auto titlelbl = new QLabel(QString::fromStdString(crt.title));
        passhlay->addWidget(titlelbl);
        titlelbl->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);

        auto idlbl = new QLabel(QString::fromStdString(crt.username));
        passhlay->addWidget(idlbl);
        idlbl->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);

        auto descriptionlbl = new QLabel(QString::fromStdString(crt.description));
        passhlay->addWidget(descriptionlbl);
        descriptionlbl->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);

        auto copyButton = new QPushButton(QIcon(":/icons/icons/copy-lightblue.svg"), " Copy");
        passhlay->addWidget(copyButton);
        copyButton->setCursor(Qt::PointingHandCursor);
        copyButton->setMinimumHeight(25);
        copyButton->setMaximumWidth(60);
        connect(copyButton, &QPushButton::clicked, [&crt, this](){ copyClicked(crt.pass); });

        auto editButton = new QPushButton(QIcon(":/icons/icons/edit-2 (1).svg"), " edit");
        passhlay->addWidget(editButton);
        editButton->setCursor(Qt::PointingHandCursor);
        editButton->setMinimumHeight(25);
        editButton->setMaximumWidth(60);
        connect(editButton, &QPushButton::clicked, [&crt, this, index](){ openEditPass(crt, index); });

        auto deleteButton = new QPushButton(QIcon(":/icons/icons/trash-2-lightblue.svg"), " delete");
        passhlay->addWidget(deleteButton);
        deleteButton->setCursor(Qt::PointingHandCursor);
        deleteButton->setMinimumHeight(25);
        deleteButton->setMaximumWidth(60);
        connect(deleteButton, &QPushButton::clicked, [&crt, this](){ deletePass(crt); });

        auto passwid = new QWidget;
        passwid->setLayout(passhlay);
        ui->passLayout->addWidget(passwid);

        ui->passwordsSubcontainer->setStyleSheet(PasswordsSubContainerStylesheet);
    }
    if(isEmpty)
    {
        ui->passwordsSubcontainer->setStyleSheet("background: transparent;");
        auto emptyhlay = new QHBoxLayout; emptyhlay->addStretch(); auto emptylbl = new QLabel("No saved passwords"); emptylbl->setStyleSheet("font: 12pt \"Segoe UI\"; color: #34699D;"); emptyhlay->addWidget(emptylbl); emptyhlay->addStretch();
        auto emptywid = new QWidget; emptywid->setLayout(emptyhlay);
        ui->passLayout->addWidget(emptywid);
    }
}

void MainWindow::openEditPass(Database::Entry &entry, int index)
{
    EditPass *editpass = new EditPass(index, db);
    editpass->exec();
    if(editpass->toSave())
    {
        entry = editpass->getEntry();
        updateDatabaseUI();
        db->save();
    }
}

void MainWindow::deletePass(Database::Entry &entry)
{
    QMessageBox box(QMessageBox::Icon::Warning, "Delete password", "Are you sure you want to delete this password?", QMessageBox::Yes | QMessageBox::No, this);
    if(box.exec() == QMessageBox::Yes)
    {
        db->remove(entry);
        updateDatabaseUI();
    }
}

void MainWindow::geometryAnimation(QWidget *target, const int &x, const int &y, const int &w, const int &h, const int &d)
{
    auto anim = new QPropertyAnimation(target, "geometry");
    anim->setDuration(d);
    anim->setStartValue(target->geometry());
    anim->setEndValue(QRect(x, y, w, h));
    anim->start();
}

void MainWindow::expandDatabaseCreateToolbar()
{
    geometryAnimation(ui->autoButton, 50, 0, 80, 40, 300);
    geometryAnimation(ui->manualButton, 140, 0, 80, 40, 300);
    createNewToggled = 1;
}

void MainWindow::shrinkDatabaseCreateToolbar()
{
    geometryAnimation(ui->autoButton, 0, 0, 40, 40, 300);
    geometryAnimation(ui->manualButton, 0, 0, 40, 40, 300);
    createNewToggled = 0;
}

void MainWindow::copyClicked(std::string s)
{
    if( timer.isActive() )
        timer.stop();
    copyToClipboard(s);
    lastClipboardItem = s;
    std::cout << "copied \n";
    std::cout.flush();
    timer.start(10000);
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
    db->save();
    updateDatabaseUI();
}

void MainWindow::homePressed()
{
    uncheckAllButtons(ui->leftMenuContainer);
    ui->stackedWidget->setCurrentWidget(ui->homePage);
}

void MainWindow::getStartedClicked()
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

void MainWindow::createNewClicked()
{
    if(!createNewToggled)
        expandDatabaseCreateToolbar();
    else
        shrinkDatabaseCreateToolbar();
}

void MainWindow::autoClicked()
{
    shrinkDatabaseCreateToolbar();
    CreateNew gen(true, db);
    gen.exec();
    if(gen.toSave())
    {
        auto title = gen.getTitle();
        auto id = gen.getID();
        auto description = gen.getDescription();
        auto password = gen.getPassword();
        db->add(Database::Entry(title, id, description, password));
        updateDatabaseUI();
    }
}

void MainWindow::manualClicked()
{
    shrinkDatabaseCreateToolbar();
    CreateNew gen(false, db);
    gen.exec();
    if(gen.toSave())
    {
        auto title = gen.getTitle();
        auto id = gen.getID();
        auto description = gen.getDescription();
        auto password = gen.getPassword();
        db->add(Database::Entry(title, id, description, password));
        updateDatabaseUI();
    }
}

void MainWindow::helpPressed()
{
    uncheckAllButtons(ui->leftMenuContainer);
    ui->stackedWidget->setCurrentWidget(ui->helpPage);
}

void MainWindow::clipboardTimedOut()
{
    timer.stop();
    if(checkClipboardContent(lastClipboardItem))
    {
        clearClipboard();
        std::cout << "timeout \n";
        std::cout.flush();
    }
    else
    {
        std::cout << "not cleared \n";
        std::cout.flush();
    }
}
