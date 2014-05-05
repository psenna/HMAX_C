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
    ui->progressBar->setValue(0);

    connect(&classes, SIGNAL(numImagensAseremProcessadas(int)), this, SLOT(setNumImagensProgresso(int)));
    connect(&classes, SIGNAL(acabouProcessarImagem()), this, SLOT(atualizaProgresso()));
    connect(&classes, SIGNAL(acabouDeProcessarAsImagens()), this, SLOT(terminouDeProcessarImagens()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

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
    classe.id = classes.classesImagens.size();
    classes.classesImagens.push_back(classe);

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
    classes.patsC1.clear();
    for(int i = 0; i < 10; i++){
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                        "",
                                                        tr("Files (*.*)"));
        classes.criaVocabulario(fileName);
    }
    C1pathDicCreator c1pc;
    c1pc.setPatchs(&classes.patsC1);
    c1pc.salvaPatchesArquivo(QString("vocabu.yml"));
}

void MainWindow::on_pushButton_4_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "",
                                                    tr("Files (*.yml)"));
    C1pathDicCreator c1pc;
    c1pc.loadPatchs(fileName);
    std::vector<patchC1>* pc1 = c1pc.getPatchs();;
    classes.patsC1.insert(classes.patsC1.end(), pc1->begin(), pc1->end());
}

void MainWindow::on_botaoRodar_clicked()
{
    //! @todo Verificar se o dicionario ja foi criado ou carregado.
    QString diretorio = QFileDialog::getExistingDirectory(this, tr("Pasta onde salvar o arquivo de classificacao"),
                                                          "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    arqSaidaSVM = diretorio + "/" + ui->nomArqSVM->text();
    classes.start();
}

void MainWindow::terminouDeProcessarImagens(){
    std::ofstream arquivo(arqSaidaSVM.toUtf8().data());
    for(std::vector<ProcessaImagem*>::iterator it = classes.threadsImagens.begin(); it != classes.threadsImagens.end(); ++it){
        std::vector<double>* respostarC2 = (*it)->respC2;
        int i = 1;
        arquivo << (*it)->classe << " ";
        for(std::vector<double>::iterator jt = respostarC2->begin(); jt != respostarC2->end(); ++jt){
            arquivo << i << ":" << (double)(*jt) << " ";
            i++;
        }
        arquivo << "\n";
        delete ((*it)->respC2);
        delete((*it));
    }
    arquivo.close();
}

void MainWindow::atualizaProgresso(){
    int valor = ui->progressBar->value();
    valor++;
    ui->progressBar->setValue(valor);
}

void MainWindow::setNumImagensProgresso(int num){
    ui->progressBar->setMaximum(num);
}

