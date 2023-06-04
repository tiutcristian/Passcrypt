#ifndef CREATENEW_H
#define CREATENEW_H

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
    std::string password;
    std::string title;
    std::string id;
    std::string description;
    bool save = 0;

    void buttonsStyle();
    void initialState(const bool &autoGenerate);
    void connectComponents();

public:
    Generate(const bool &autoGenerate);
    ~Generate();

    std::string getPassword(){ return password; }
    std::string getTitle(){ return title; }
    std::string getID(){ return id; }
    std::string getDescription(){ return description; }
    bool toSave(){ return save; }

public slots:
    void descriptionChanged();
    void saveClicked();
    void cancelClicked();
};
#endif // CREATENEW_H
