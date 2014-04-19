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
    std::vector<S1_T>* resultados;
    std::vector<C1_T>* resultadosC1;
    cv::Mat imagem = cv::imread("lena.jpg");
    cvtColor(imagem, imagem, CV_BGR2GRAY);

    cv::imshow("Tela", imagem);
    cv::waitKey(0);

    return 0;
}
