#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "logindialog.h"
#include "signupdialog.h"
#include "database.h"
#include "editpass.h"
#include <QMainWindow>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:

    // general variables
    Ui::MainWindow *ui;
    Database *db;
    QTimer timer;
    std::string lastClipboardItem;

    // fast generate page variables
    bool upperToggled = 1;
    bool lowerToggled = 1;
    bool numbersToggled = 1;
    bool symbolsToggled = 1;

    // database page variables
    bool createNewToggled = 0;

    // general methods
    void initialDialog();
    void connectComponents();
    void buttonStyle();
    void initialState();
    void uncheckAllButtons(QObject* widget);
    void geometryAnimation(QWidget* target, const int &x, const int &y, const int &w, const int &h, const int &d);

    //fast generate page methods
    void charOptionsClicked(bool &toggled, QWidget *widget);

    // database page methods
    void updateDatabaseUI();
    void expandDatabaseCreateToolbar();
    void shrinkDatabaseCreateToolbar();
    void copyClicked(std::string s);
    void editClicked(Database::Entry &entry, int index);
    void deleteClicked(Database::Entry &entry);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // general variables
    loginDialog ld;
    signupDialog sd;

    // database page methods
    void clearLayout(QLayout *layout);

    //general mathods
    void setDatabase(Database* db);

public slots:

    // general
    void clipboardTimedOut();

    // home page
    void homePressed();
    void getStartedClicked();

    // fast generate page
    void zapPressed();
    void fastCopyClicked();
    void generateClicked();

    // database page
    void databasePressed();
    void createNewClicked();
    void autoClicked();
    void manualClicked();

    // help page
    void helpPressed();
};
#endif // MAINWINDOW_H
