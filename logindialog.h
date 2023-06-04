#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class loginDialog;
}

class loginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit loginDialog(QWidget *parent = nullptr);
    ~loginDialog();

private:
    Ui::loginDialog *ui;
    void buttonStyle();
    void connectComponents();

private slots:
    void confirmClicked();
};

#endif // LOGINDIALOG_H
