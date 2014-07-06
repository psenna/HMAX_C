/**
 * Executa a camada S2 e C2 do modelo HMAX
 *
 *
 * Pedro Senna
 * Ciências da computação
 * Universidade Federal de Itajubá - UNIFEI
 *
 */

#ifndef C2TH_H
#define C2TH_H

#include <QThread>
#include "baseStructs.h"

class C2th : public QThread
{
    Q_OBJECT
public:
    explicit C2th(QObject *parent = 0);
    explicit C2th(std::vector<patchC1> *patchs, std::vector<C1_T> *C1output, float sigma,
                  float alpha,QObject *parent = 0);

    std::vector<float> *estimulos;

    void roda();

private:
    std::vector<patchC1> *patchs;
    std::vector<C1_T>    *C1output;
    float sigma;
    float alpha;

#ifdef CUDAON
    cv::gpu::Stream stream;
#endif

};

#endif // C2TH_H
