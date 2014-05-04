#include "processaimagem.h"
#include "HMax_Class/s1th.h"
#include "HMax_Class/c1th.h"
#include "HMax_Class/c1pathdiccreator.h"
#include "HMax_Class/c2th.h"

ProcessaImagem::ProcessaImagem(QObject *parent) :
    QThread(parent)
{
}

ProcessaImagem::ProcessaImagem(QString nomeImagem,
                               int classe,
                               std::vector<cv::Mat>* filtrosGaborS1,
                               std::vector<int>*     tamanhosS1,
                               std::vector<double>*  lambdaS1,
                               std::vector<double>*  sigmaS1,
                               std::vector<double>*  gamaS1,
                               std::vector<double>*  orientacaoS1,
                               std::vector<int>* tamanhoC1,
                               std::vector<int>* overlapC1,
                               std::vector<patchC1>* patsC1,
                               QObject *parent){

    this->nomeImagem = nomeImagem;
    this->classe = classe;
    this->filtrosGaborS1 = filtrosGaborS1;
    this->tamanhosS1 = tamanhosS1;
    this->lambdaS1 = lambdaS1;
    this->sigmaS1 = sigmaS1;
    this->gamaS1 = gamaS1;
    this->orientacaoS1 = orientacaoS1;
    this->tamanhoC1 = tamanhoC1;
    this->overlapC1 = overlapC1;
    this->patsC1 = patsC1;
}

void ProcessaImagem::run(){
    cv::Mat imagem = cv::imread(nomeImagem.toUtf8().data());
    cvtColor(imagem, imagem, CV_BGR2GRAY);

    // S1
    S1Th s1(imagem, tamanhosS1, lambdaS1, sigmaS1, gamaS1, orientacaoS1, filtrosGaborS1);
    s1.start();
    s1.wait();
    this->respS1 = s1.gaborFilterResult;

    // C1
    C1th c1(tamanhoC1, overlapC1, respS1);
    c1.start();
    c1.wait();
    this->respC1 = c1.resultado;

    if(patsC1 != NULL){
        // Realizar as camadas S2 e C2
        C2th c2(patsC1, respC1, 1, (tamMenorPat/4)*(tamMenorPat/4));
        c2.start();
        c2.wait();
        respC2 =  c2.estimulos;
    } else {
        // criar os patchs C1
        std::vector<int> tamanhos;
        std::vector<int> numero;
        tamanhos.push_back(2);
        numero.push_back(96);
        tamanhos.push_back(4);
        numero.push_back(96);
        tamanhos.push_back(8);
        numero.push_back(96);
        tamanhos.push_back(12);
        numero.push_back(96);
        C1pathDicCreator p1(respC1, &tamanhos, &numero);
        p1.start();
        p1.wait();
        patsC1 = p1.getPatchs();
    }
}


