#ifndef BASESTRUCTS_H
#define BASESTRUCTS_H

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#define PI 3.1415926536

struct S1_T{
  cv::Mat imgFiltrada;
  int tamanho;
  double orientation;
};

struct C1_T{
  cv::Mat imgMaxBand;
  int tamanho;
  int overlap;
  double orientation;
};

#endif // BASESTRUCTS_H
