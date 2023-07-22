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
#include <cstdlib>
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
    connect(ui->plusButton, SIGNAL(pressed()), this, SLOT(zapPressed()));
    connect(ui->databaseButton, SIGNAL(pressed()), this, SLOT(databasePressed()));    
    connect(ui->settingsButton, SIGNAL(pressed()), this, SLOT(settingsPressed()));
    connect(ui->helpButton, SIGNAL(pressed()), this, SLOT(helpPressed()));

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

    // settings page text changed
    connect(ui->changePasswordCurrentPasswordLineEdit, &QLineEdit::textChanged, [=]{ style()->polish(ui->changePasswordCurrentPasswordLineEdit); hideMessageLabels(); });
    connect(ui->changePasswordNewPasswordLineEdit, &QLineEdit::textChanged, [=]{ style()->polish(ui->changePasswordNewPasswordLineEdit); hideMessageLabels(); });
    connect(ui->changePasswordReNewPasswordLineEdit, &QLineEdit::textChanged, [=]{ style()->polish(ui->changePasswordReNewPasswordLineEdit); hideMessageLabels(); });

    // settings page buttons
    connect(ui->changePasswordButton, SIGNAL(clicked()), this, SLOT(changePasswordClicked()));

    // timer
    connect(&timer, SIGNAL(timeout()), this, SLOT(clipboardTimedOut()));
}

void MainWindow::buttonStyle()
{
    // left menu
    ui->plusButton->setCursor(Qt::PointingHandCursor);
    ui->databaseButton->setCursor(Qt::PointingHandCursor);
    ui->settingsButton->setCursor(Qt::PointingHandCursor);
    ui->helpButton->setCursor(Qt::PointingHandCursor);

    // fast generate page
    ui->copyButton->setCursor(Qt::PointingHandCursor);
    ui->upperButton->setCursor(Qt::PointingHandCursor);
    ui->lowerButton->setCursor(Qt::PointingHandCursor);
    ui->numbersButton->setCursor(Qt::PointingHandCursor);
    ui->symbolsButton->setCursor(Qt::PointingHandCursor);
    ui->generateButton->setCursor(Qt::PointingHandCursor);

    // database page
    ui->newButton->setCursor(Qt::PointingHandCursor);
    ui->autoButton->setCursor(Qt::PointingHandCursor);
    ui->manualButton->setCursor(Qt::PointingHandCursor);

    // setting page
    ui->changePasswordButton->setCursor(Qt::PointingHandCursor);
}

void MainWindow::initialState()
{
    // general
    uncheckAllButtons(ui->leftMenuContainer);
    ui->stackedWidget->setCurrentWidget(ui->homePage);
    ui->progressBar->hide();
    ui->progressBarLabel->hide();

    // settings page
    hideMessageLabels();
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
        // copy current entry to crt
        auto &crt = db->entries[index];

        // create new horizontal layout
        auto passhlay = new QHBoxLayout;
        passhlay->setSpacing(10);

        // title label
        auto titlelbl = new QLabel(QString::fromStdString(crt.title));
        passhlay->addWidget(titlelbl);
        titlelbl->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);

        // id label
        auto idlbl = new QLabel(QString::fromStdString(crt.username));
        passhlay->addWidget(idlbl);
        idlbl->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);

        // description label
        auto descriptionlbl = new QLabel(QString::fromStdString(crt.description));
        passhlay->addWidget(descriptionlbl);
        descriptionlbl->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);

        // copy button
        auto copyButton = new QPushButton(QIcon(":/icons/icons/copy-lightblue.svg"), "");
        passhlay->addWidget(copyButton);
        copyButton->setCursor(Qt::PointingHandCursor);
        copyButton->setMinimumHeight(30);
        copyButton->setMaximumHeight(30);
        copyButton->setMinimumWidth(30);
        copyButton->setMaximumWidth(30);
        connect(copyButton, &QPushButton::clicked, [&crt, this](){ copyClicked(crt.pass); });

        // edit button
        auto editButton = new QPushButton(QIcon(":/icons/icons/edit-2 (1).svg"), "");
        passhlay->addWidget(editButton);
        editButton->setCursor(Qt::PointingHandCursor);
        editButton->setMinimumHeight(30);
        editButton->setMaximumHeight(30);
        editButton->setMinimumWidth(30);
        editButton->setMaximumWidth(30);
        connect(editButton, &QPushButton::clicked, [&crt, this, index](){ editClicked(crt, index); });

        // delete button
        auto deleteButton = new QPushButton(QIcon(":/icons/icons/trash-2-lightblue.svg"), "");
        passhlay->addWidget(deleteButton);
        deleteButton->setCursor(Qt::PointingHandCursor);
        deleteButton->setMinimumHeight(30);
        deleteButton->setMaximumHeight(30);
        deleteButton->setMinimumWidth(30);
        deleteButton->setMaximumWidth(30);
        connect(deleteButton, &QPushButton::clicked, [&crt, this](){ deleteClicked(crt); });

        // create widget
        auto passwid = new QWidget;
        passwid->setLayout(passhlay);

        // add widget to passLayout
        ui->passLayout->addWidget(passwid);

        // mark that passLayout is not empty and set default stylesheet
        isEmpty = false;
        ui->passwordsSubcontainer->setStyleSheet(PasswordsSubContainerStylesheet);
    }

    // if there is no password in the database
    if(isEmpty)
    {
        // set respective stylesheet
        ui->passwordsSubcontainer->setStyleSheet("background: transparent;");

        // create new horizaontal layout
        auto emptyhlay = new QHBoxLayout;
        emptyhlay->addStretch();
        auto emptylbl = new QLabel("No saved passwords");
        emptylbl->setStyleSheet("font: 12pt \"Segoe UI\"; color: #34699D;");
        emptyhlay->addWidget(emptylbl);
        emptyhlay->addStretch();

        // create widget
        auto emptywid = new QWidget;
        emptywid->setLayout(emptyhlay);

        // add widget to passLayout
        ui->passLayout->addWidget(emptywid);
    }
}

void MainWindow::copyClicked(std::string s)
{
    if( timer.isActive() )
        timer.stop();
    ui->progressBar->setValue(100);
    ui->progressBarLabel->setText("10 seconds");
    ui->progressBar->show();
    ui->progressBarLabel->show();
    copyToClipboard(s);
    lastClipboardItem = s;
    timer.start(1000);
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

void MainWindow::hideMessageLabels()
{
    ui->changePasswordUpdatedLabel->hide();
    ui->changePasswordWrongCurrentLabel->hide();
    ui->changePasswordNotMatchLabel->hide();
}

void MainWindow::geometryAnimation(QWidget *target, const int &x, const int &y, const int &w, const int &h, const int &duration)
{
    auto anim = new QPropertyAnimation(target, "geometry");
    anim->setDuration(duration);
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

    // create new password based on char options and length
    char_options co;
    co.lowercase = lowerToggled;
    co.uppercase = upperToggled;
    co.numbers = numbersToggled;
    co.symbols = symbolsToggled;
    int length = atoi( ui->lengthLineEdit->text().toStdString().c_str() );
    std::string newPass = generatePassword(length, co);

    // print password on the QLineEdit
    ui->passwordLineEdit_2->setText(QString::fromStdString(newPass));
    ui->passwordLineEdit_2->setDisabled(1);
    ui->passwordLineEdit_2->setCursorPosition(0);

    // switch to fast generate page
    ui->stackedWidget->setCurrentWidget(ui->fastGeneratePage);
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

void MainWindow::settingsPressed()
{
    uncheckAllButtons(ui->leftMenuContainer);
    ui->stackedWidget->setCurrentWidget(ui->settingsPage);
}

void MainWindow::changePasswordClicked()
{
    hideMessageLabels();
    if(ui->changePasswordNewPasswordLineEdit->text() != ui->changePasswordReNewPasswordLineEdit->text())
        ui->changePasswordNotMatchLabel->show();
    else
    {
        try
        {
            auto verify = decrypt("database.txt", ui->changePasswordCurrentPasswordLineEdit->text().toStdString());
            db->updateMasterPassword(ui->changePasswordNewPasswordLineEdit->text().toStdString());
            db->save();
            ui->changePasswordUpdatedLabel->show();
        }
        catch (decryptionError)
        {
            ui->changePasswordWrongCurrentLabel->show();
        }
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
    int val = ui->progressBar->value();
    if(val > 10)
    {
        ui->progressBar->setValue(val-10);
        int nrOfSeconds = (val-10)/10;
        string str_nrOfSeconds = std::to_string(nrOfSeconds);
        ui->progressBarLabel->setText( QString::fromStdString( str_nrOfSeconds + " seconds" ) );
        timer.start(1000);
    }
    else
    {
        ui->progressBar->hide();
        ui->progressBarLabel->hide();
        if(checkClipboardContent(lastClipboardItem))
            clearClipboard();
    }
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
    ui->progressBar->setValue(100);
    ui->progressBarLabel->setText("10 seconds");
    ui->progressBar->show();
    ui->progressBarLabel->show();
    copyToClipboard(s);
    lastClipboardItem = s;
    timer.start(1000);
}

void MainWindow::generateClicked()
{
    // create new password based on char options and length
    char_options co;
    co.lowercase = lowerToggled;
    co.uppercase = upperToggled;
    co.numbers = numbersToggled;
    co.symbols = symbolsToggled;
    int length = atoi( ui->lengthLineEdit->text().toStdString().c_str() );
    std::string newPass = generatePassword(length, co);

    // print password on the QLineEdit
    ui->passwordLineEdit_2->setText(QString::fromStdString(newPass));
    ui->passwordLineEdit_2->setDisabled(1);
    ui->passwordLineEdit_2->setCursorPosition(0);
}
