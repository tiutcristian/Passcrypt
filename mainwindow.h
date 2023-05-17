#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "logindialog.h"
#include "signupdialog.h"
#include "database.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    Database* db;

    void initialDialog();
    void connectComponents();
    void buttonStyle();
    void initialState();
    void uncheckAllButtons(QObject* widget);
    //DbEntry toDbEntry(const char name[], const char id[], const char desc[], const char pass[]);
    //void addToDatabase(const DbEntry & elem);
    //void getDatabase();
    //void updateDatabaseFile();
    void updateDatabaseUI();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    loginDialog ld;
    signupDialog sd;
    void clearLayout(QLayout *layout);
    void setDatabase(Database* db);

public slots:
    void homePressed();
    void getStartedPressed();
    void databasePressed();
    void createNewPressed();
    void helpPressed();
};
#endif // MAINWINDOW_H
