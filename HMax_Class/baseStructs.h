/**
 * Estruturas usadas na comunicaçao entre as camadas
 * do programa.
 *
 * Pedro Senna
 * Ciências da computação
 * Universidade Federal de Itajubá - UNIFEI
 *
 */
//#define CUDAON

#ifdef CUDAON
#include <opencv2/gpu/gpu.hpp>
#include <opencv2/gpu/gpumat.hpp>
#endif

#ifndef BASESTRUCTS_H
#define BASESTRUCTS_H

#include <vector>
#include <stdlib.h>
#include <cmath>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#define PI 3.1415926536


// *********************  Param. HMAX  ************************
#define USAHMAX false
#define KVOC    1000
#define CLUSTERIZAVOC true

// Parametros gerais
#define nOrientacoes 12
#define tamMenorPat  4
#define TAMMINIMOIMG 240

// Parametros C2
//#define nOrientacoesC2 nOrientacoes
#define nOrientacoesC2 1
// De acordo com o tamanho da amostra
#define AJUSTEGAUSSIANA 500000000


// *********************   PARAM. BOF ************************
#define USABOF true
#define KVOCBOF 500
#define FREAK_ON
//#define GOODDETECTOR

#define AJUSTEGAUSSIANABOF 500000000
#define BOFMAX

struct S1_T{
#ifdef  CUDAON2
    cv::gpu::GpuMat imgFiltrada[nOrientacoes];
#else
    cv::Mat imgFiltrada[nOrientacoes];
#endif
    int tamanho;
    float orientation[nOrientacoes];
};

struct C1_T{
#ifdef CUDAON
    cv::gpu::GpuMat imgMaxBand[nOrientacoes];
#else
    cv::Mat imgMaxBand[nOrientacoesC2];
#endif
};

struct patchC1{
#ifdef CUDAON
    cv::gpu::GpuMat patch[nOrientacoes];
#else
    cv::Mat patch[nOrientacoesC2];
#endif
};

struct S2_T{
    cv::Mat respostaAosEstimulos;
    patchC1 patch;
};


#define Filter_T cv::Mat



#endif // BASESTRUCTS_H
