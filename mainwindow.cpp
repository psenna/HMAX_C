#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Configura a tablewidget
    ui->tableWidget->insertColumn(0);
    ui->tableWidget->insertColumn(1);
    ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Classe"));
    ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("N imagens"));
    ui->tableWidget->horizontalHeader()->sectionResizeMode(QHeaderView::Stretch);

    iniciaParametros();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::iniciaParametros(){
    tamanhosS1.push_back(7);
    tamanhosS1.push_back(9);
    tamanhosS1.push_back(11);
    tamanhosS1.push_back(13);
    tamanhosS1.push_back(15);
    tamanhosS1.push_back(17);
    tamanhosS1.push_back(19);
    tamanhosS1.push_back(21);
    tamanhosS1.push_back(23);
    tamanhosS1.push_back(25);
    tamanhosS1.push_back(27);
    tamanhosS1.push_back(29);
    tamanhosS1.push_back(31);
    tamanhosS1.push_back(33);
    tamanhosS1.push_back(35);
    tamanhosS1.push_back(37);

    lambdaS1.push_back(2.8);
    lambdaS1.push_back(3.6);
    lambdaS1.push_back(4.5);
    lambdaS1.push_back(5.4);
    lambdaS1.push_back(6.3);
    lambdaS1.push_back(7.3);
    lambdaS1.push_back(8.2);
    lambdaS1.push_back(9.2);
    lambdaS1.push_back(10.2);
    lambdaS1.push_back(11.3);
    lambdaS1.push_back(12.3);
    lambdaS1.push_back(13.4);
    lambdaS1.push_back(14.6);
    lambdaS1.push_back(15.8);
    lambdaS1.push_back(17.0);
    lambdaS1.push_back(18.2);

    sigmaS1.push_back(3.5);
    sigmaS1.push_back(4.6);
    sigmaS1.push_back(5.6);
    sigmaS1.push_back(6.8);
    sigmaS1.push_back(7.9);
    sigmaS1.push_back(9.1);
    sigmaS1.push_back(10.3);
    sigmaS1.push_back(11.5);
    sigmaS1.push_back(12.7);
    sigmaS1.push_back(14.1);
    sigmaS1.push_back(15.4);
    sigmaS1.push_back(16.8);
    sigmaS1.push_back(18.2);
    sigmaS1.push_back(19.7);
    sigmaS1.push_back(21.2);
    sigmaS1.push_back(22.8);

    gamaS1.push_back(0.3);
    gamaS1.push_back(0.3);
    gamaS1.push_back(0.3);
    gamaS1.push_back(0.3);
    gamaS1.push_back(0.3);
    gamaS1.push_back(0.3);
    gamaS1.push_back(0.3);
    gamaS1.push_back(0.3);
    gamaS1.push_back(0.3);
    gamaS1.push_back(0.3);
    gamaS1.push_back(0.3);
    gamaS1.push_back(0.3);
    gamaS1.push_back(0.3);
    gamaS1.push_back(0.3);
    gamaS1.push_back(0.3);
    gamaS1.push_back(0.3);

    orientacaoS1.push_back(0);
    orientacaoS1.push_back(PI/4);
    orientacaoS1.push_back(PI/2);
    orientacaoS1.push_back(3*PI/4);

    tamanhoC1.push_back(8);
    tamanhoC1.push_back(10);
    tamanhoC1.push_back(12);
    tamanhoC1.push_back(14);
    tamanhoC1.push_back(16);
    tamanhoC1.push_back(18);
    tamanhoC1.push_back(20);
    tamanhoC1.push_back(22);

    overlapC1.push_back(4);
    overlapC1.push_back(5);
    overlapC1.push_back(6);
    overlapC1.push_back(7);
    overlapC1.push_back(8);
    overlapC1.push_back(9);
    overlapC1.push_back(10);
    overlapC1.push_back(11);



}

/*
void MainWindow::on_pushButton_clicked()
{
    QString directory = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                          "",
                                                          QFileDialog::ShowDirsOnly
                                                          | QFileDialog::DontResolveSymlinks);
    QMessageBox::information(this, tr("Arquivo aberto"), directory);
}

void MainWindow::on_botao_abrir_arquivo_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "~/",
                                                    tr("Files (*.*)"));
    QMessageBox::information(this, tr("Arquivo aberto"), fileName);
}*/

void MainWindow::on_botaoNovClas_clicked()
{
    QString directory = QFileDialog::getExistingDirectory(this, tr("Pasta onde esta a classe"),
                                                          "",
                                                          QFileDialog::ShowDirsOnly
                                                          | QFileDialog::DontResolveSymlinks);
    classeImagem classe;
    classe.caminho = directory;
    classe.nome = ui->lineEdit->text();
    ui->lineEdit->setText("");
    classe.numImgs = ui->spinBox->value();
    classe.id = this->classesImagens.size();
    this->classesImagens.push_back(classe);

    ui->tableWidget->insertRow(classe.id);
    ui->tableWidget->setItem(classe.id, 0, new QTableWidgetItem(classe.nome));
    ui->tableWidget->setItem(classe.id, 1, new QTableWidgetItem(QString().sprintf("%d",classe.numImgs)));

}

void MainWindow::on_actionSair_triggered()
{
    qApp->exit(0);
}

void MainWindow::on_botaoVoc_clicked()
{
    this->patsC1.clear();
    for(int i = 0; i < 10; i++){
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                        "",
                                                        tr("Files (*.*)"));
        ProcessaImagem p1(fileName, 0, NULL, &tamanhosS1, &lambdaS1, &sigmaS1, &gamaS1, &orientacaoS1, &tamanhoC1, &overlapC1, NULL);
        p1.start();
        p1.wait();
        this->patsC1.insert(patsC1.end(), p1.patsC1->begin(), p1.patsC1->end());
    }
    C1pathDicCreator c1pc;
    c1pc.setPatchs(&patsC1);
    c1pc.salvaPatchesArquivo(QString("vocabu.yml"));
}
