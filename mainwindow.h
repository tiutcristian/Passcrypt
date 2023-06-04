#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "logindialog.h"
#include "signupdialog.h"
#include "database.h"
#include "editpass.h"
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
    bool createNewToggled = 0;

    void initialDialog();
    void connectComponents();
    void buttonStyle();
    void initialState();
    void uncheckAllButtons(QObject* widget);
    void updateDatabaseUI();
    void copyToClipboard(const QString &text);
    void openEditPass(Database::Entry &entry);
    void geometryAnimation(QWidget* target, const int &x, const int &y, const int &w, const int &h, const int &d);
    void expandDatabaseCreateToolbar();
    void shrinkDatabaseCreateToolbar();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    loginDialog ld;
    signupDialog sd;
    void clearLayout(QLayout *layout);
    void setDatabase(Database* db);

public slots:
    void homePressed();
    void getStartedClicked();
    void databasePressed();
    void createNewClicked();
    void autoClicked();
    void manualClicked();
    void helpPressed();
};
#endif // MAINWINDOW_H
