#include "c1th.h"

C1th::C1th(QObject *parent) :
    QThread(parent)
{
}

C1th::C1th(std::vector<int> *tamanho, std::vector<int> *overlap, std::vector<S1_T>* imagensS1, QObject *parent):
    QThread(parent)
{
    this->tamanho = tamanho;
    this->overlap = overlap;
    this->imagensS1 = imagensS1;
}

void C1th::roda(){
    this->resultado = new std::vector<C1_T>;
    resultado->resize(imagensS1->size());

    std::vector<S1_T>::iterator imgS1 = imagensS1->begin();

    for(std::vector<C1_T>::iterator it = resultado->begin(); it != resultado->end(); ++it){
        it->tamanho = TAMANHOC1;
        it->overlap = AMOSTRAGEMC1;

        // Cria o elemento da dilatação
        cv::Mat element = cv::getStructuringElement( cv::MORPH_RECT,
                                                     cv::Size( TAMANHOC1, TAMANHOC1 ),
                                                     cv::Point( floor(TAMANHOC1/2), floor(TAMANHOC1/2)));

        for(int i = 0; i < nOrientacoes; i++){

            // Utiliza a opeação Dilatação
#ifdef CUDAON
            cv::gpu::dilate(imgS1->imgFiltrada[i], it->imgMaxBand[i], element);

#else
            cv::dilate(imgS1->imgFiltrada[i], it->imgMaxBand[i], element);

#endif

            // Amostragem dos pontos
            int rows = ceil(it->imgMaxBand[i].rows / AMOSTRAGEMC1);
            int coluns = ceil(it->imgMaxBand[i].cols / AMOSTRAGEMC1);
#ifdef CUDAON
            cv::gpu::resize(it->imgMaxBand[i], it->imgMaxBand[i], cv::Size(coluns, rows), 0, 0, cv::INTER_NEAREST);
#else
            cv::resize(it->imgMaxBand[i], it->imgMaxBand[i], cv::Size(coluns, rows), 0, 0, cv::INTER_NEAREST);
#endif
        }
        imgS1++;
    }
}
