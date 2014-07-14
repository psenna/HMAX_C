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
    cv::gpu::MatchTemplateBuf buf;
    cv::gpu::GpuMat bufMinMax;
    cv::gpu::GpuMat aux2;
#else
    cv::Mat aux;
    cv::Mat soma;
#endif

    double menor = DBL_MAX;
    double min, max;

    for(std::vector<patchC1>::iterator i = patchs->begin(); i != patchs->end(); ++i){
        menor = DBL_MAX;
        for(std::vector<C1_T>::iterator j = C1output->begin(); j != C1output->end(); ++j){
            if(i->patch[0].cols < j->imgMaxBand[0].cols && i->patch[0].rows < j->imgMaxBand[0].rows && i->patch[0].rows > 0 && i->patch[0].cols > 0){
#ifndef CUDAON
                soma = cv::Mat::zeros(j->imgMaxBand[0].rows - i->patch[0].rows + 1, j->imgMaxBand[0].cols - i->patch[0].cols + 1, CV_32F);
#endif
                for(int k = 0; k < nOrientacoesC2; k++){
                    aux.create(j->imgMaxBand[0].rows - i->patch[0].rows + 1, j->imgMaxBand[0].cols - i->patch[0].cols + 1, CV_32F);
#ifdef CUDAON
                    cv::gpu::matchTemplate(j->imgMaxBand[k], i->patch[k], aux, CV_TM_SQDIFF, buf, stream);
                    if(!k){
                        soma.create(aux.size(), aux.type());
                        stream.enqueueCopy(aux, soma);
                    } else {
                        cv::gpu::add(soma, aux, soma, cv::gpu::GpuMat(), -1, stream);
                    }
#else
                    cv::matchTemplate(j->imgMaxBand[k], i->patch[k], aux, CV_TM_SQDIFF);
                    cv::add(soma, aux, soma);
#endif
                }
#ifdef CUDAON
                stream.waitForCompletion();
                cv::gpu::minMax(soma, &min, &max, cv::gpu::GpuMat(), bufMinMax);
#else
                cv::minMaxLoc(soma, &min, &max, NULL, NULL, cv::Mat());
#endif
                if(min < menor)
                    menor = min;
            }
        }
<<<<<<< HEAD
         *est = (float) cv::exp((-(menor)/(AJUSTEGAUSSIANA*sigma*sigma*alpha)));
=======
        menor /= (i->patch[0].rows * i->patch[0].cols * nOrientacoesC2);
        *est = (float) cv::exp((-(menor)/(AJUSTEGAUSSIANA*sigma*sigma*alpha)));
>>>>>>> FETCH_HEAD
        est++;
    }
}
