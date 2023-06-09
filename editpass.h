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
    // EditPass(QWidget *parent = nullptr);
    explicit EditPass(const Database::Entry &entry, Database *db);
    ~EditPass();

    Database::Entry getEntry(){ return entry; }
    bool toSave(){ return save; }

private:
    Ui::EditPass *ui;
    Database::Entry entry;
    Database *db;
    bool save;
    bool editButtonClicked = 0;

    void buttonStyle();
    void connectComponents();
    void initialState();

private slots:
    void descriptionChanged();
    void editClicked();
    void saveClicked();
    void cancelClicked();
};

#endif // EDITPASS_H
