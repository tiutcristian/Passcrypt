#ifndef EDITPASS_H
#define EDITPASS_H

#include <QDialog>
#include <QTimer>
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
    QTimer timer;

    // toggle buttons
    bool editButtonToggled = 0;

    // general methods
    void buttonStyle();
    void connectComponents();
    void initialState();

private slots:

    // text changed slots
    void descriptionChanged();
    void passwordChanged();

    // button-clicked slots
    void editClicked();
    void saveClicked();
    void cancelClicked();

    // timer slot
    void editTimedOut();
};

#endif // EDITPASS_H
