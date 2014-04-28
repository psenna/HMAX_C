#include "adicionaclasse.h"
#include "ui_adicionaclasse.h"

adicionaClasse::adicionaClasse(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::adicionaClasse)
{
    ui->setupUi(this);
}

adicionaClasse::~adicionaClasse()
{
    delete ui;
}
