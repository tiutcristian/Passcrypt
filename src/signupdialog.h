#ifndef SIGNUPDIALOG_H
#define SIGNUPDIALOG_H

#include <QDialog>

namespace Ui {
class signupDialog;
}

class signupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit signupDialog(QWidget *parent = nullptr);
    ~signupDialog();

private:
    Ui::signupDialog *ui;
    void buttonStyle();
    void connectComponents();
    bool confirmedClicked = false;

private slots:
    void confirmClicked();
    void window_closed();
};

#endif // SIGNUPDIALOG_H
