#ifndef EDITPASS_H
#define EDITPASS_H

#include <QDialog>

namespace Ui {
class EditPass;
}

class EditPass : public QDialog
{
    Q_OBJECT

public:
    // EditPass(QWidget *parent = nullptr);
    explicit EditPass(const QString &name, const QString &id, const QString &description, const QString &password);
    ~EditPass();

    QString getName(){ return name; }
    QString getID(){ return id; }
    QString getDescription(){ return description; }
    QString getPassword(){ return password; }
    bool toSave(){ return save; }

private:
    Ui::EditPass *ui;
    QString name;
    QString id;
    QString description;
    QString password;
    bool save;

    void buttonStyle();
    void connectComponents();
    void initialState();

private slots:
    void descriptionChanged();
    void savePressed();
};

#endif // EDITPASS_H
