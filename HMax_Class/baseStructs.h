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
#define nOrientacoes 12
#define tamMenorPat 2


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
#ifdef CUDAON2
    cv::gpu::GpuMat imgMaxBand[nOrientacoes];
#else
    cv::Mat imgMaxBand[nOrientacoes];
#endif
    int tamanho;
    int overlap;
    float orientation[nOrientacoes];
};

struct patchC1{
#ifdef CUDAON
    cv::gpu::GpuMat patch[nOrientacoes];
#else
    cv::Mat patch[nOrientacoes];
#endif
};

struct S2_T{
    cv::Mat respostaAosEstimulos;
    patchC1 patch;
};


#define Filter_T cv::Mat



#endif // BASESTRUCTS_H
