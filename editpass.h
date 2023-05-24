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
    explicit EditPass(QWidget *parent = nullptr);
    EditPass(const QString &name, const QString &id, const QString &description, const QString &password);
    ~EditPass();

private:
    Ui::EditPass *ui;
    QString name;
    QString id;
    QString description;
    QString password;

    void buttonStyle();
    void connectComponents();
};

#endif // EDITPASS_H
