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
                               std::vector<Filter_T>* filtrosGaborS1,
                               std::vector<int>*     tamanhosS1,
                               std::vector<float>*  lambdaS1,
                               std::vector<float>*  sigmaS1,
                               std::vector<float>*  gamaS1,
                               std::vector<float>*  orientacaoS1,
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

void ProcessaImagem::roda(){
    cv::Mat imagem = cv::imread(nomeImagem.toUtf8().data());
    cvtColor(imagem, imagem, CV_BGR2GRAY);
    imagem.convertTo(imagem, CV_32F);

    clock_t t = clock();
    // S1
    S1Th s1(imagem, tamanhosS1, lambdaS1, sigmaS1, gamaS1, orientacaoS1, filtrosGaborS1);
    s1.roda();
    this->respS1 = s1.gaborFilterResult;

    std::cout << "S1 " << (int)(clock() - t) << "\n";
    t = clock();

    // C1
    C1th c1(tamanhoC1, overlapC1, respS1);
    c1.roda();
    this->respC1 = c1.resultado;
    delete(this->respS1);

    std::cout << "C1 " << (int)(clock() - t) << "\n";
    t = clock();

    if(patsC1 != NULL){
        // Realizar as camadas S2 e C2
        C2th c2(patsC1, respC1, 1, (float)((tamMenorPat/4.0)*(tamMenorPat/4.0)));
        c2.roda();
        respC2 =  c2.estimulos;
        delete(respC1);
    } else {
        // criar os patchs C1
        std::vector<int> tamanhos;
        std::vector<int> numero;
        tamanhos.push_back(4);
        numero.push_back(10 * 15);
        tamanhos.push_back(8);
        numero.push_back(10 * 15);
        tamanhos.push_back(12);
        numero.push_back(10 * 15);
        tamanhos.push_back(16);
        numero.push_back(10 * 15);

        C1pathDicCreator p1(respC1, &tamanhos, &numero);
        p1.start();
        p1.wait();
        patsC1 = p1.getPatchs();
    }
    std::cout << "C2 " << (int)(clock() - t) << "\n";
}
