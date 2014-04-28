#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QString>
#include <iostream>
#include "estruturas.h"
#include "processaimagem.h"

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

private:
    Ui::MainWindow *ui;
    std::vector<classeImagem> classesImagens;

    void iniciaParametros();

    // Parametros do programa.
    // Vetores de parametros S1
    std::vector<int>     tamanhosS1;
    std::vector<double>  lambdaS1;
    std::vector<double>  sigmaS1;
    std::vector<double>  gamaS1;
    std::vector<double>  orientacaoS1;

    // Vetores de parametros C1
    std::vector<int> tamanhoC1;
    std::vector<int> overlapC1;

    std::vector<cv::Mat> filtrosGaborS1;
    std::vector<patchC1> patsC1;


};

#endif // MAINWINDOW_H
