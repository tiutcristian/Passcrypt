#ifndef CREATENEW_H
#define CREATENEW_H

#include <QDialog>
#include <string>
#include "database.h"

namespace Ui {
class CreateNew;
}

class CreateNew : public QDialog
{
    Q_OBJECT

private:
    Ui::CreateNew *ui;
    std::string password;
    std::string title;
    std::string id;
    std::string description;
    bool save = 0;
    Database *db;
    bool advancedToggled = 0;

    void buttonsStyle();
    void initialState(const bool &autoGenerate);
    void connectComponents();
    void expandAdvancedMenu();
    void shrinkAdvancedMenu();

public:
    CreateNew(const bool &autoGenerate, Database *db);
    ~CreateNew();

    std::string getPassword(){ return password; }
    std::string getTitle(){ return title; }
    std::string getID(){ return id; }
    std::string getDescription(){ return description; }
    bool toSave(){ return save; }

public slots:
    void descriptionChanged();
    void advancedClicked();
    void saveClicked();
    void cancelClicked();
};
#endif // CREATENEW_H
