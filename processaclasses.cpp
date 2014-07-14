#include "processaclasses.h"

ProcessaClasses::ProcessaClasses(QObject *parent) :
    QThread(parent)
{
    iniciaParametros();
}

void ProcessaClasses::iniciaParametros(){
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
    lambdaS1.push_back(17);
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

    for(int i = 0; i < nOrientacoes; i++){
        orientacaoS1.push_back(PI*i/nOrientacoes);
    }

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

    S1Th s1(cv::Mat(), &tamanhosS1, &lambdaS1, &sigmaS1, &gamaS1, &orientacaoS1, NULL);
    s1.criaFiltro();
    filtrosGaborS1.insert(filtrosGaborS1.end(), s1.filters->begin(), s1.filters->end());
    delete(s1.filters);
}

void ProcessaClasses::criaVocabulario(){
    this->patsC1.clear();
    for(std::vector<classeImagem>::iterator it = this->classesImagens.begin(); it != this->classesImagens.end(); ++it){
        srand(time(NULL));
        QStringList nameFilter("*.jpg");
        QDir directory(it->caminho);
        QStringList imageFiles = directory.entryList(nameFilter);

        for(int i = 0; imageFiles.size() && i < it->numImgs; i++){
            int imgSorteada = rand() % imageFiles.size();
            QString arquivo = imageFiles.at(imgSorteada);
            imageFiles.removeAt(imgSorteada);
            arquivo = it->caminho + "/" + arquivo;
            ProcessaImagem *img = new ProcessaImagem(arquivo, it->id, &filtrosGaborS1, &tamanhosS1,
                                                     &lambdaS1, &sigmaS1, &gamaS1, &orientacaoS1,
                                                     &tamanhoC1, &overlapC1, NULL);
            img->roda();
            this->patsC1.insert(patsC1.end(), img->patsC1->begin(), img->patsC1->end());
            delete(img);
        }
    }
}
void ProcessaClasses::criaVocabularioBOF(){
    for(std::vector<classeImagem>::iterator it = this->classesImagens.begin(); it != this->classesImagens.end(); ++it){
        srand(time(NULL));
        QStringList nameFilter("*.jpg");
        QDir directory(it->caminho);
        QStringList imageFiles = directory.entryList(nameFilter);
        this->vocabularioBOF = cv::Mat();
        for(int i = 0; imageFiles.size() && i < it->numImgs; i++){
            int imgSorteada = rand() % imageFiles.size();
            QString arquivo = imageFiles.at(imgSorteada);
            imageFiles.removeAt(imgSorteada);
            arquivo = it->caminho + "/" + arquivo;
            Bof bof(arquivo, NULL);
            if(this->vocabularioBOF.rows == 0){
                this->vocabularioBOF = bof.extraiCaract();
            } else {
                cv::vconcat(this->vocabularioBOF, bof.extraiCaract(), this->vocabularioBOF);
            }
        }
    }
    cv::kmeans(this->vocabularioBOF, KVOC, cv::Mat(), cv::TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 10000, 0.0001), 5, cv::KMEANS_PP_CENTERS, this->vocabularioBOF);
}

void ProcessaClasses::run(){
    int numImagens = 0;
    for(std::vector<classeImagem>::iterator it = this->classesImagens.begin(); it != this->classesImagens.end(); ++it){
        numImagens += it->numImgs;
    }
    emit numImagensAseremProcessadas(numImagens);

    for(std::vector<classeImagem>::iterator it = this->classesImagens.begin(); it != this->classesImagens.end(); ++it){
        srand(time(NULL));
        QStringList nameFilter("*.jpg");
        QDir directory(it->caminho);
        QStringList imageFiles = directory.entryList(nameFilter);

        for(int i = 0; imageFiles.size() && i < it->numImgs; i++){
            int imgSorteada = rand() % imageFiles.size();
            QString arquivo = imageFiles.at(imgSorteada);
            imageFiles.removeAt(imgSorteada);
            arquivo = it->caminho + "/" + arquivo;
            ProcessaImagem *img = new ProcessaImagem(arquivo, it->id, &filtrosGaborS1, &tamanhosS1,
                                                     &lambdaS1, &sigmaS1, &gamaS1, &orientacaoS1,
                                                     &tamanhoC1, &overlapC1, &patsC1);
            this->threadsImagens.push_back(img);
            img->roda();
            emit acabouProcessarImagem();
        }
    }

    emit acabouDeProcessarAsImagens();
}


