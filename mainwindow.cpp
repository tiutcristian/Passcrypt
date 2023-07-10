#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logindialog.h"
#include "signupdialog.h"
#include "createnew.h"
#include "editpass.h"
#include "encrypt.h"
#include "clipboard.h"
#include "constants.h"

#include <fstream>
//#include <iostream>
#include <QStyle>
#include <QPropertyAnimation>
#include <QLineEdit>
#include <QSizePolicy>
#include <QMessageBox>

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
    // left menu buttons
    connect(ui->homeButton, SIGNAL(pressed()), this, SLOT(homePressed()));
    connect(ui->plusButton, SIGNAL(pressed()), this, SLOT(zapPressed()));
    connect(ui->databaseButton, SIGNAL(pressed()), this, SLOT(databasePressed()));
    connect(ui->helpButton, SIGNAL(pressed()), this, SLOT(helpPressed()));

    // home page buttons
    connect(ui->getStartedButton, SIGNAL(clicked()), this, SLOT(getStartedClicked()));

    // fast generate page buttons
    connect(ui->copyButton, SIGNAL(clicked()), this, SLOT(fastCopyClicked()));
    connect(ui->upperButton, &QPushButton::clicked, [=](){ charOptionsClicked(upperToggled, ui->upperButton); });
    connect(ui->lowerButton, &QPushButton::clicked, [=](){ charOptionsClicked(lowerToggled, ui->lowerButton); });
    connect(ui->numbersButton, &QPushButton::clicked, [=](){ charOptionsClicked(numbersToggled, ui->numbersButton); });
    connect(ui->symbolsButton, &QPushButton::clicked, [=](){ charOptionsClicked(symbolsToggled, ui->symbolsButton); });
    connect(ui->generateButton, SIGNAL(clicked()), this, SLOT(generateClicked()));

    // database page buttons
    connect(ui->newButton, SIGNAL(clicked()), this, SLOT(createNewClicked()));
    connect(ui->autoButton, SIGNAL(clicked()), this, SLOT(autoClicked()));
    connect(ui->manualButton, SIGNAL(clicked()), this, SLOT(manualClicked()));

    //other
    connect(&timer, SIGNAL(timeout()), this, SLOT(clipboardTimedOut()));
}

void MainWindow::buttonStyle()
{
    std::vector<QPushButton*> v;

    // left menu buttons
    v.push_back(ui->homeButton);
    v.push_back(ui->plusButton);
    v.push_back(ui->databaseButton);
    v.push_back(ui->helpButton);

    // home page buttons
    v.push_back(ui->getStartedButton);

    // fast generate page buttons
    v.push_back(ui->copyButton);
    v.push_back(ui->upperButton);
    v.push_back(ui->lowerButton);
    v.push_back(ui->numbersButton);
    v.push_back(ui->symbolsButton);
    v.push_back(ui->generateButton);

    // database page buttons
    v.push_back(ui->newButton);
    v.push_back(ui->autoButton);
    v.push_back(ui->manualButton);

    for(auto crt : v)
        crt->setCursor(Qt::PointingHandCursor);
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
        connect(editButton, &QPushButton::clicked, [&crt, this, index](){ editClicked(crt, index); });

        auto deleteButton = new QPushButton(QIcon(":/icons/icons/trash-2-lightblue.svg"), " delete");
        passhlay->addWidget(deleteButton);
        deleteButton->setCursor(Qt::PointingHandCursor);
        deleteButton->setMinimumHeight(25);
        deleteButton->setMaximumWidth(60);
        connect(deleteButton, &QPushButton::clicked, [&crt, this](){ deleteClicked(crt); });

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

void MainWindow::copyClicked(std::string s)
{
    if( timer.isActive() )
        timer.stop();
    copyToClipboard(s);
    lastClipboardItem = s;
    //std::cout << "copied \n";
    //std::cout.flush();
    timer.start(10000);
}

void MainWindow::editClicked(Database::Entry &entry, int index)
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

void MainWindow::deleteClicked(Database::Entry &entry)
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

void MainWindow::zapPressed()
{
    uncheckAllButtons(ui->leftMenuContainer);
    char_options co;
    co.lowercase = lowerToggled;
    co.uppercase = upperToggled;
    co.numbers = numbersToggled;
    co.symbols = symbolsToggled;
    int length = atoi( ui->lengthLineEdit->text().toStdString().c_str() );
    std::string newPass = generatePassword(length, co);
    ui->passwordLineEdit_2->setText(QString::fromStdString(newPass));
    ui->passwordLineEdit_2->setDisabled(1);
    ui->passwordLineEdit_2->setCursorPosition(0);
    ui->stackedWidget->setCurrentWidget(ui->plusPage);
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
    //{
        clearClipboard();
    //  std::cout << "timeout \n";
    //  std::cout.flush();
    //}
    //else
    //{
    //    std::cout << "not cleared \n";
    //    std::cout.flush();
    //}
}

void MainWindow::charOptionsClicked(bool &toggled, QWidget *widget)
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

void MainWindow::fastCopyClicked()
{
    if( timer.isActive() )
        timer.stop();
    std::string s = ui->passwordLineEdit_2->text().toStdString();
    copyToClipboard(s);
    lastClipboardItem = s;
    //std::cout << "copied \n";
    //std::cout.flush();
    timer.start(10000);
}

void MainWindow::generateClicked()
{
    char_options co;
    co.lowercase = lowerToggled;
    co.uppercase = upperToggled;
    co.numbers = numbersToggled;
    co.symbols = symbolsToggled;
    int length = atoi( ui->lengthLineEdit->text().toStdString().c_str() );
    std::string newPass = generatePassword(length, co);
    ui->passwordLineEdit_2->setText(QString::fromStdString(newPass));
    ui->passwordLineEdit_2->setDisabled(1);
    ui->passwordLineEdit_2->setCursorPosition(0);
}
