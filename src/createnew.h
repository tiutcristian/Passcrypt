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

    // general variables
    Ui::CreateNew *ui;
    Database *db;
    bool save = 0;

    // strings containing entry data
    std::string password;
    std::string title;
    std::string id;
    std::string description;

    // toggle buttons
    bool advancedToggled = 0;
    bool upperToggled = 1;
    bool lowerToggled = 1;
    bool numbersToggled = 1;
    bool symbolsToggled = 1;

    //general methods
    void buttonsStyle();
    void initialState(const bool &autoGenerate);
    void connectComponents();

    // advanced menu methods
    void expandAdvancedMenu();
    void shrinkAdvancedMenu();
    void charOptionClicked(bool &toggled, QWidget *widget);

public:
    CreateNew(bool autoGenerate, Database *db);
    ~CreateNew();

    // functions providing entry data
    std::string getPassword(){ return password; }
    std::string getTitle(){ return title; }
    std::string getID(){ return id; }
    std::string getDescription(){ return description; }
    bool toSave(){ return save; }

public slots:

    // general slots
    void saveClicked();
    void cancelClicked();

    // text changed slots
    void descriptionChanged();

    // advanced menu slots
    void advancedClicked();
    void regenerateClicked();
};
#endif // CREATENEW_H
