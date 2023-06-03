#ifndef SAVE_PASS_DIALOG2_H
#define SAVE_PASS_DIALOG2_H

#include <QDialog>
#include <string>

namespace Ui {
class save_pass_dialog2;
}

class save_pass_dialog2 : public QDialog
{
    Q_OBJECT

public:
    explicit save_pass_dialog2(QWidget *parent = nullptr);
    ~save_pass_dialog2();
    std::string getName(){ return name; }
    std::string getID(){ return ID; }
    std::string getDescription(){ return description; }
    bool toSave(){ return save; }

private:
    Ui::save_pass_dialog2 *ui;
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

#endif // SAVE_PASS_DIALOG2_H
