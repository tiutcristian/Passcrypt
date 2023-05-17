#ifndef SAVE_PASS_DIALOG_H
#define SAVE_PASS_DIALOG_H

#include <QDialog>
#include <string>

namespace Ui {
class save_pass_dialog;
}

class save_pass_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit save_pass_dialog(QWidget *parent = nullptr);
    ~save_pass_dialog();
    std::string getName(){ return name; }
    std::string getID(){ return ID; }
    std::string getDescription(){ return description; }
    bool toSave(){ return save; }

private:
    Ui::save_pass_dialog *ui;
    void connectComponents();
    void buttonsStyle();
    void paintEvent(QPaintEvent *event);
    std::string name;
    std::string ID;
    std::string description;
    bool save;

private slots:
    void savePressed();
    void cancelPressed();
    void descriptionChanged();
};

#endif // SAVE_PASS_DIALOG_H
