#ifndef EDITPASS_H
#define EDITPASS_H

#include <QWidget>

namespace Ui {
class EditPass;
}

class EditPass : public QWidget
{
    Q_OBJECT

public:
    explicit EditPass(QWidget *parent = nullptr);
    ~EditPass();

private:
    Ui::EditPass *ui;
};

#endif // EDITPASS_H
