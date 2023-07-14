#ifndef EDITPASS_H
#define EDITPASS_H

#include <QDialog>
#include "database.h"

namespace Ui {
class EditPass;
}

class EditPass : public QDialog
{
    Q_OBJECT

public:

    // constructor
    explicit EditPass(int index, Database *db);
    ~EditPass();

    // functions providing data
    Database::Entry getEntry(){ return entry; }
    bool toSave(){ return save; }

private:

    // general variables
    Ui::EditPass *ui;
    Database::Entry entry;
    int index;
    Database *db;
    bool save;

    // toggle buttons
    bool editButtonToggled = 0;

    // general methods
    void buttonStyle();
    void connectComponents();
    void initialState();

private slots:

    // text changed slots
    void descriptionChanged();

    // button-clicked slots
    void editClicked();
    void saveClicked();
    void cancelClicked();
};

#endif // EDITPASS_H
