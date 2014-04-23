#ifndef BASESTRUCTS_H
#define BASESTRUCTS_H

#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#define PI 3.1415926536
#define nOrientacoes 4


struct S1_T{
    cv::Mat imgFiltrada[nOrientacoes];
    int tamanho;
    double orientation[nOrientacoes];
};

struct C1_T{
    cv::Mat imgMaxBand[nOrientacoes];
    int tamanho;
    int overlap;
    double orientation[nOrientacoes];
};

struct patchC1{
    cv::Mat patch[nOrientacoes];
};

struct S2_T{
    cv::Mat respostaAosEstimulos;
    patchC1 patch;
};

#endif // BASESTRUCTS_H
