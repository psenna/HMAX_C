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

void MainWindow::on_actionSair_triggered()
{
    qApp->exit(0);
}

void MainWindow::on_botaoVoc_clicked()
{
    classes.patsC1.clear();
    classes.criaVocabulario();
    C1pathDicCreator c1pc;
    c1pc.setPatchs(&classes.patsC1);
    if(CLUSTERIZAVOC)
        c1pc.clusteriza();
    c1pc.salvaPatchesArquivo(QString("vocabulario.yml"));
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
    tempo = clock();
    //! @todo Verificar se o dicionario ja foi criado ou carregado.
    QString diretorio = QFileDialog::getExistingDirectory(this, tr("Pasta onde salvar o arquivo de classificacao"),
                                                          "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    arqSaidaSVM = diretorio + "/" + ui->nomArqSVM->text();
    classes.start();
    ui->botaoRodar->setEnabled(false);
}

void MainWindow::terminouDeProcessarImagens(){
    if(USAHMAX){
        QString saida = arqSaidaSVM + "Hmax.svm";
        std::ofstream arquivo(saida.toUtf8().data());
        for(std::vector<ProcessaImagem*>::iterator it = classes.threadsImagens.begin(); it != classes.threadsImagens.end(); ++it){
            std::vector<float>* respostarC2 = (*it)->respC2;
            int i = 1;
            arquivo << (*it)->classe << " ";
            for(std::vector<float>::iterator jt = respostarC2->begin(); jt != respostarC2->end(); ++jt){
                arquivo << i << ":" << (float)(*jt) << " ";
                i++;
            }
            arquivo << "\n";
            delete ((*it)->respC2);
            delete((*it));
        }
    }

    if(USABOF){
        QString saida = arqSaidaSVM + "BOF.svm";
        std::ofstream arquivo(saida.toUtf8().data());
        for(std::vector<Bof*>::iterator it = classes.thBof.begin(); it != classes.thBof.end(); ++it){
            arquivo << (*it)->classe << " ";
            cv::Mat aux = (*it)->getHistograma();
            for(int i = 0; i < aux.cols; i++){
                arquivo << i << ":" << (float)(aux.at<float>(i)) << " ";
            }
            arquivo << "\n";
        }
        std::cout << "Arquivo " << arqSaidaSVM.toUtf8().data() << " gravado com sucesso.\n";
        arquivo.close();
    }
    ui->botaoRodar->setEnabled(true);
    ui->progressBar->setMaximum(1);
    ui->progressBar->setValue(1);
    tempo = clock() - tempo;
    std::cout << "Tempo: " << ((float)tempo)/CLOCKS_PER_SEC << "\n";
}

void MainWindow::atualizaProgresso(){
    int valor = ui->progressBar->value();
    valor++;
    ui->progressBar->setValue(valor);
}

void MainWindow::setNumImagensProgresso(int num){
    ui->progressBar->setMaximum(num);
}


void MainWindow::on_pushButton_clicked()
{
    QString diretorio = QFileDialog::getExistingDirectory(this, tr("Diretorio com as classes separadas em pastas"),
                                                          "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    QDir directory(diretorio);
    directory.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    QStringList nomesDiretorios = directory.entryList();
    int imgPorClasses = ui->NumImgPClasse->text().toInt();
    for(QStringList::iterator i = nomesDiretorios.begin(); i != nomesDiretorios.end(); ++i){
        classeImagem classe;
        classe.caminho = diretorio + "/" +*i;
        classe.nome = *i;

        classe.numImgs = imgPorClasses;
        classe.id = classes.classesImagens.size();
        classes.classesImagens.push_back(classe);

        ui->tableWidget->insertRow(classe.id);
        ui->tableWidget->setItem(classe.id, 0, new QTableWidgetItem(classe.nome));
        ui->tableWidget->setItem(classe.id, 1, new QTableWidgetItem(QString().sprintf("%d",classe.numImgs)));
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    this->classes.criaVocabularioBOF();
}

void MainWindow::on_pushButton_5_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "",
                                                    tr("Files (*.yml)"));
    Bof bof(QString(), NULL);
    classes.vocabularioBOF = bof.loadVoc(fileName);
}
