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

    overlapC1.push_back(4);
    overlapC1.push_back(5);
    overlapC1.push_back(6);
    overlapC1.push_back(7);
    overlapC1.push_back(8);
    overlapC1.push_back(9);

    S1Th s1(cv::Mat(), &tamanhosS1, &lambdaS1, &sigmaS1, &gamaS1, &orientacaoS1, NULL);
    s1.criaFiltro();
    filtrosGaborS1.insert(filtrosGaborS1.end(), s1.filters->begin(), s1.filters->end());
    delete(s1.filters);
    nThreadRodando = 0;
    this->mutex.lock();
    this->mutex.unlock();
}

void ProcessaClasses::criaVocabulario(QString imagem){
    ProcessaImagem p1(imagem, 0, NULL, &tamanhosS1, &lambdaS1, &sigmaS1, &gamaS1, &orientacaoS1, &tamanhoC1, &overlapC1, NULL);
    p1.start();
    p1.wait();
    this->patsC1.insert(patsC1.end(), p1.patsC1->begin(), p1.patsC1->end());
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
            while(1){
                mutex.lock();
                if(nThreadRodando < MAXTHREADS){
                    nThreadRodando++;
                    mutex.unlock();
                    break;
                }
                mutex.unlock();
            }
            connect(img, SIGNAL(finished()), this, SLOT(acabouThread()));
            img->start();
        }
    }



    for(std::vector<ProcessaImagem*>::iterator it = this->threadsImagens.begin(); it != this->threadsImagens.end(); ++it){
        (*it)->wait();
    }

    emit acabouDeProcessarAsImagens();
}

void ProcessaClasses::acabouThread(){
    mutex.lock();
    nThreadRodando--;
    mutex.unlock();
    emit acabouProcessarImagem();
}


