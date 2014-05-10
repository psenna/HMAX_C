#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QString>
#include <iostream>
#include <fstream>
#include "estruturas.h"
#include "processaclasses.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_botaoNovClas_clicked();
    void on_actionSair_triggered();

    void on_botaoVoc_clicked();

    void on_pushButton_4_clicked();

    void on_botaoRodar_clicked();

    void on_pushButton_clicked();

public slots:
    void terminouDeProcessarImagens();
    void atualizaProgresso();
    void setNumImagensProgresso(int num);

private:
    Ui::MainWindow *ui;
    ProcessaClasses classes;
    QString arqSaidaSVM;


};

#endif // MAINWINDOW_H
