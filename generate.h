#ifndef GENERATE_H
#define GENERATE_H

#include <QDialog>
#include <string>

namespace Ui {
class Generate;
}

class Generate : public QDialog
{
    Q_OBJECT

private:
    Ui::Generate *ui;
    std::string encryptedPassword;
    std::string passwordName;
    std::string passwordID;
    std::string passwordDescription;
    bool save;

    void buttonsStyle();
    void initialState();
    void connectComponents();
    void paintEvent(QPaintEvent *event);

public:
    explicit Generate(QWidget *parent = nullptr);
    ~Generate();

    bool upper_pressed = 0;
    bool lower_pressed = 0;
    bool numbers_pressed = 0;
    bool symbols_pressed = 0;

    void expand();

    std::string getEncryptedPassword(){ return encryptedPassword; }
    std::string getName(){ return passwordName; }
    std::string getID(){ return passwordID; }
    std::string getDescription(){ return passwordDescription; }
    bool toSave(){ return save; }

public slots:
    void encryptPressed();
    void clipboardPressed();
    void yesPressed();
    void noPressed();
    void xPressed();
};
#endif // GENERATE_H
