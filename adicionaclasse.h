#ifndef ADICIONACLASSE_H
#define ADICIONACLASSE_H

#include <QDialog>

namespace Ui {
class adicionaClasse;
}

class adicionaClasse : public QDialog
{
    Q_OBJECT

public:
    explicit adicionaClasse(QWidget *parent = 0);
    ~adicionaClasse();

private:
    Ui::adicionaClasse *ui;
};

#endif // ADICIONACLASSE_H
