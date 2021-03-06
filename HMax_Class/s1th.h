/**
 * Executa a camada S1 do modelo HMAX
 *
 * Pedro Senna
 * Ciências da computação
 * Universidade Federal de Itajubá - UNIFEI
 *
 */

#ifndef S1TH_H
#define S1TH_H

#include <QThread>
#include <vector>
#include "HMax_Class/baseStructs.h"

class S1Th : public QThread
{
    Q_OBJECT
public:
    explicit S1Th(QObject *parent = 0);
    explicit S1Th(cv::Mat image, std::vector<int>* sizes, std::vector<float>* lambda, std::vector<float>* sigma,
                  std::vector<float>* gama, std::vector<float>* orientation, std::vector<cv::Mat>* filters, QObject *parent = 0);

    std::vector<S1_T>* gaborFilterResult;
    std::vector<cv::Mat>* filters;
    void roda();
    void criaFiltro();

private:
    cv::Mat image;
    std::vector<int>* sizes;
    std::vector<float>* lambda;
    std::vector<float>* sigma;
    std::vector<float>* gama;
    std::vector<float>* orientation;
    void run();
};

#endif // S1TH_H
