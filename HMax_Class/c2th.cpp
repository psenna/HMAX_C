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

void C2th::run(){/*
    this->estimulos = new std::vector<float>;
    this->estimulos->resize(patchs->size());
    for(std::vector<float>::iterator i = estimulos->begin(); i != estimulos->end(); ++i)
        *i = 0;

    std::vector<float>::iterator est = estimulos->begin();
    cv::Mat aux;
    float auxEsp;

    for(std::vector<patchC1>::iterator i = patchs->begin(); i != patchs->end(); ++i){
        for(std::vector<C1_T>::iterator j = C1output->begin(); j != C1output->end(); ++j){
            int tamanhox = j->imgMaxBand[0].cols;
            int tamanhoy = j->imgMaxBand[0].rows;
            int deslocx = i->patch[0].cols;
            int deslocy = i->patch[0].rows;
            int fimx = tamanhox - deslocx;
            int fimy = tamanhoy - deslocy;
            if(deslocx < tamanhox && deslocy < tamanhoy){
                for(int k = 0; k < fimy; k++){
                    for(int l = 0; l < fimx; l++){
                        auxEsp = 0.0;
                        for(int m = 0; m < nOrientacoes; m++){
                            cv::Rect roi(l, k, deslocx, deslocy);
                            cv::Mat crop(j->imgMaxBand[m], roi);
                            cv::absdiff(crop, i->patch[m], aux);
                            cv::Scalar soma = cv::sum(aux);
                            auxEsp += soma[0];
                        }
                        auxEsp = auxEsp / (float)(deslocx*deslocy);
                        auxEsp = cv::exp((-(auxEsp)/(2.0*sigma*sigma*alpha)));
                        if(auxEsp > *est)
                            *est = auxEsp;
                    }
                }
            }
        }
        est++;
    }*/
}

void C2th::roda(){
    this->estimulos = new std::vector<float>;
    this->estimulos->resize(patchs->size());
    for(std::vector<float>::iterator i = estimulos->begin(); i != estimulos->end(); ++i)
        *i = 0.0;

    std::vector<float>::iterator est = estimulos->begin();
    cv::Mat aux;
    float auxEsp;

    for(std::vector<patchC1>::iterator i = patchs->begin(); i != patchs->end(); ++i){
        for(std::vector<C1_T>::iterator j = C1output->begin(); j != C1output->end(); ++j){
            float min = std::numeric_limits<float>::max();
            int tamanhox = j->imgMaxBand[0].cols;
            int tamanhoy = j->imgMaxBand[0].rows;
            int deslocx = i->patch[0].cols;
            int deslocy = i->patch[0].rows;
            int fimx = tamanhox - deslocx;
            int fimy = tamanhoy - deslocy;
            if(deslocx < tamanhox && deslocy < tamanhoy){
                for(int k = 0; k < fimy; k++){
                    for(int l = 0; l < fimx; l++){
                        auxEsp = 0.0;
                        for(int m = 0; m < nOrientacoes; m++){
                            cv::Rect roi(l, k, deslocx, deslocy);
                            cv::Mat crop(j->imgMaxBand[m], roi);
                            cv::absdiff(crop, i->patch[m], aux);
                            cv::Scalar soma = cv::sum(aux);
                            auxEsp += soma[0];
                        }
                        auxEsp = auxEsp / (float)(deslocx*deslocy*nOrientacoes);
                        auxEsp = cv::exp((-(auxEsp)/(1000.0*sigma*sigma*alpha)));
                        if(auxEsp > *est)
                            *est = auxEsp;
                    }
                }
            }
        }
        est++;
    }
    std::cout << "\n";
}
