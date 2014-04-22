#include "mainwindow.h"
#include <QApplication>
#include "HMax_Class/s1th.h"

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
    orientacao.push_back(33 * PI / 180);

    S1Th s1(imagem, &tamanhos, &lambda, &sigma, &gama, &orientacao, NULL);
    s1.start();
    s1.wait();

    std::vector<S1_T>* resultados = s1.gaborFilterResult;

    for(int i = 0; i < (int)resultados->size(); i++){
        cv::imshow("Tela", resultados->at(i).imgFiltrada);
        std::cout << "S1 " <<  resultados->at(i).tamanho << "  " << resultados->at(i).orientation << "\n";
        cv::waitKey(0);
    }

    return 0;
}
