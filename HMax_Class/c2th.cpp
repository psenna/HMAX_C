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
    cv::Mat aux;
    cv::Mat soma;
    double menor = DBL_MAX;

    for(std::vector<patchC1>::iterator i = patchs->begin(); i != patchs->end(); ++i){
        menor = DBL_MAX;
        for(std::vector<C1_T>::iterator j = C1output->begin(); j != C1output->end(); ++j){
            if(i->patch[0].cols < j->imgMaxBand[0].cols && i->patch[0].rows < j->imgMaxBand[0].rows){
                soma = cv::Mat::zeros(j->imgMaxBand[0].rows - i->patch[0].rows + 1, j->imgMaxBand[0].cols - i->patch[0].cols + 1, CV_32F);
                for(int k = 0; k < nOrientacoes; k++){
                    cv::matchTemplate(j->imgMaxBand[k], i->patch[k], aux, CV_TM_SQDIFF);
                    cv::add(soma, aux, soma);
                }
                double min, max;
                cv::minMaxLoc(soma, &min, &max, NULL, NULL, cv::Mat());
                if(min < menor)
                    menor = min;
            }
        }
        *est = (float) cv::exp((-(menor)/(500000000*sigma*sigma*alpha)));
        est++;
    }
}
