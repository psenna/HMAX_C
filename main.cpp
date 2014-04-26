#include "mainwindow.h"
#include <QApplication>
#include "HMax_Class/s1th.h"
#include "HMax_Class/c1th.h"
#include "HMax_Class/c1pathdiccreator.h"

int main(int argc, char *argv[])
{
    //QApplication a(argc, argv);
    //MainWindow w;
    //w.show();

    //return a.exec();

    cv::namedWindow("Tela",1);
    cv::Mat imagem = cv::imread("lena.jpg");
    cvtColor(imagem, imagem, CV_BGR2GRAY);

    cv::imshow("Tela", imagem);
    cv::waitKey(0);

    //parametros
    std::vector<int>   tamanhos;
    tamanhos.push_back(7);
    tamanhos.push_back(9);

    std::vector<double> lambda;
    lambda.push_back(3.5);
    lambda.push_back(4.6);

    std::vector<double> sigma;
    sigma.push_back(2.8);
    sigma.push_back(3.6);

    std::vector<double> gama;
    gama.push_back(0.3);
    gama.push_back(0.3);

    std::vector<double> orientacao;
    orientacao.push_back(0.0 * PI / 180.0);
    orientacao.push_back(45.0 * PI / 180.0);
    orientacao.push_back(90.0 * PI / 180.0);
    orientacao.push_back(135.0 * PI / 180.0);

    S1Th s1(imagem, &tamanhos, &lambda, &sigma, &gama, &orientacao, NULL);
    s1.start();
    s1.wait();

    std::vector<S1_T>* resultados = s1.gaborFilterResult;

    for(int i = 0; i < (int)resultados->size(); i++){
        for(int j = 0; j < nOrientacoes; j++){
            cv::imshow("Tela", resultados->at(i).imgFiltrada[j]);
            std::cout << "S1 " <<  resultados->at(i).tamanho << "  " << resultados->at(i).orientation[j] << "  " << resultados->at(i).imgFiltrada[j].size() << "\n";
            cv::waitKey(0);
        }
    }

    std::vector<int> tamanhoC1;
    tamanhoC1.push_back(8);

    std::vector<int> overlapC1;
    overlapC1.push_back(4);

    C1th c1(&tamanhoC1, &overlapC1, resultados);

    c1.start();
    c1.wait();

    std::vector<C1_T>* resultadosC1 = c1.resultado;
    std::vector<patchC1>* pat = new std::vector<patchC1>;
    pat->resize(resultadosC1->size());
    std::vector<patchC1>::iterator itPat = pat->begin();

    for(std::vector<C1_T>::iterator it = resultadosC1->begin(); it != resultadosC1->end(); it++){
        for(int i = 0; i < nOrientacoes; i++){
            itPat->patch[i] = it->imgMaxBand[i];
            cv::imshow("Tela", it->imgMaxBand[i]);
            std::cout << "C1 " << it->tamanho << "  " << it->imgMaxBand[i].size() << "\n";
            cv::waitKey(0);
        }
        itPat++;
    }

    C1pathDicCreator c1pat;
    c1pat.setPatchs(pat);
    c1pat.salvaPatchesArquivo(QString("teste.yml"));
    delete(resultados);
    delete(resultadosC1);
    delete(pat);
    return 0;
}
