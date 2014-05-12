/**
 * Estruturas usadas na comunicaçao entre as camadas
 * do programa.
 *
 * Pedro Senna
 * Ciências da computação
 * Universidade Federal de Itajubá - UNIFEI
 *
 */

#ifndef BASESTRUCTS_H
#define BASESTRUCTS_H

#include <vector>
#include <stdlib.h>
#include <cmath>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#define PI 3.1415926536
#define nOrientacoes 12
#define tamMenorPat 2


struct S1_T{
    cv::Mat imgFiltrada[nOrientacoes];
    int tamanho;
    float orientation[nOrientacoes];
};

struct C1_T{
    cv::Mat imgMaxBand[nOrientacoes];
    int tamanho;
    int overlap;
    float orientation[nOrientacoes];
};

struct patchC1{
    cv::Mat patch[nOrientacoes];
};

struct S2_T{
    cv::Mat respostaAosEstimulos;
    patchC1 patch;
};


#endif // BASESTRUCTS_H
