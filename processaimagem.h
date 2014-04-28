#ifndef PROCESSAIMAGEM_H
#define PROCESSAIMAGEM_H

#include <QThread>
#include "HMax_Class/s1th.h"
#include "HMax_Class/c1th.h"
#include "HMax_Class/c1pathdiccreator.h"
#include "HMax_Class/c2th.h"

class ProcessaImagem : public QThread
{
    Q_OBJECT
public:
    explicit ProcessaImagem(QObject *parent = 0);
    explicit ProcessaImagem(QString nomeImagem,
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
                            QObject *parent = 0
    );

private:
    void run();

    QString nomeImagem;
    int classe;

    // Vetores de parametros S1
    std::vector<int>*     tamanhosS1;
    std::vector<double>*  lambdaS1;
    std::vector<double>*  sigmaS1;
    std::vector<double>*  gamaS1;
    std::vector<double>*  orientacaoS1;

    // Vetores de parametros C1
    std::vector<int>* tamanhoC1;
    std::vector<int>* overlapC1;

public:
    // Vetores de parametros S1
    std::vector<cv::Mat>* filtrosGaborS1;

    // Parametro S2 ou saida para criar os patsC1
    std::vector<patchC1>* patsC1;

    // Saidas
    std::vector<C1_T>* respC1;
    std::vector<S1_T>* respS1;
    std::vector<double>* respC2;

};

#endif // PROCESSAIMAGEM_H
