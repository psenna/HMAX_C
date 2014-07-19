#ifndef PROCESSACLASSES_H
#define PROCESSACLASSES_H

#include <QThread>
#include <QString>
#include <iostream>
#include <QDir>
#include <qstringlist.h>
#include <QBasicMutex>
#include "estruturas.h"
#include "processaimagem.h"
#include "BOF/bof.h"

#define MAXTHREADS 1

class ProcessaClasses : public QThread
{
    Q_OBJECT
public:
    explicit ProcessaClasses(QObject *parent = 0);
    std::vector<classeImagem> classesImagens;

    void iniciaParametros();

    // Parametros do programa.
    // Vetores de parametros S1
    std::vector<int>     tamanhosS1;
    std::vector<float>  lambdaS1;
    std::vector<float>  sigmaS1;
    std::vector<float>  gamaS1;
    std::vector<float>  orientacaoS1;

    // Vetores de parametros C1
    std::vector<int> tamanhoC1;
    std::vector<int> overlapC1;

    std::vector<cv::Mat> filtrosGaborS1;
    std::vector<patchC1> patsC1;

    std::vector<ProcessaImagem*> threadsImagens;
    std::vector<Bof*> thBof;

    //Parametros BOF
    cv::Mat vocabularioBOF;

    void criaVocabulario();
    void criaVocabularioBOF();
    void run();

public slots:

signals:
    void acabouProcessarImagem();
    void numImagensAseremProcessadas(int num);
    void acabouDeProcessarAsImagens();

};

#endif // PROCESSACLASSES_H
