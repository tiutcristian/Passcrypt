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
    bool confirmedClicked = false;

private slots:
    void confirmClicked();
    void window_closed();
};

#endif // LOGINDIALOG_H
