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

    void initialDialog();
    void connectComponents();
    void buttonStyle();
    void initialState();
    void uncheckAllButtons(QObject* widget);
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
    void editPressed(const QString &name, const QString &id, const QString &description, const QString &password);
};
#endif // MAINWINDOW_H
