#include "c2th.h"

C2th::C2th(QObject *parent) :
    QThread(parent)
{
}

C2th::C2th(std::vector<patchC1> *patchs, std::vector<C1_T> *C1output, float sigma, float alpha, QObject *parent){
    this->patchs = patchs;
    this->C1output = C1output;
    this->alpha = alpha;
    this->sigma = sigma;
    this->estimulos = NULL;
}


void C2th::roda(){
    this->estimulos = new std::vector<float>;
    this->estimulos->resize(patchs->size());
    for(std::vector<float>::iterator i = estimulos->begin(); i != estimulos->end(); ++i)
        *i = 0.0;

    std::vector<float>::iterator est = estimulos->begin();
#ifdef CUDAON
    cv::gpu::GpuMat aux;
    cv::gpu::GpuMat soma;
#else
    cv::Mat aux;
    cv::Mat soma;
#endif

    for(std::vector<patchC1>::iterator i = patchs->begin(); i != patchs->end(); ++i){
        for(std::vector<C1_T>::iterator j = C1output->begin(); j != C1output->end(); ++j){
            if(i->patch[0].cols < j->imgMaxBand[0].cols && i->patch[0].rows < j->imgMaxBand[0].rows){
#ifndef CUDAON
                soma = cv::Mat::zeros(j->imgMaxBand[0].rows - i->patch[0].rows + 1, j->imgMaxBand[0].cols - i->patch[0].cols + 1, CV_32F);
#endif
                for(int k = 0; k < nOrientacoes; k++){
#ifdef CUDAON
                    cv::gpu::matchTemplate(j->imgMaxBand[k], i->patch[k], aux, CV_TM_SQDIFF_NORMED);
                    if(!k){
                        soma = aux;
                    } else {
                        cv::gpu::add(soma, aux, soma);
                    }
#else
                    cv::matchTemplate(j->imgMaxBand[k], i->patch[k], aux, CV_TM_SQDIFF);
                    cv::add(soma, aux, soma);
#endif
                }
                double min, max;
#ifdef CUDAON
                cv::gpu::minMaxLoc(soma, &min, &max, NULL, NULL, cv::gpu::GpuMat());
#else
                cv::minMaxLoc(soma, &min, &max, NULL, NULL, cv::Mat());
#endif
                *est = (float) cv::exp((-(min)/(5000000000*sigma*sigma*alpha)));
            }
        }
        est++;
    }
}
