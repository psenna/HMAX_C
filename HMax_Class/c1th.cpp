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
    resultado->resize((int)floor(imagensS1->size()/2));

    std::vector<int>::iterator tam = tamanho->begin();
    std::vector<int>::iterator over = overlap->begin();
    std::vector<S1_T>::iterator imgS1 = imagensS1->begin();
    std::vector<S1_T>::iterator imgS1_2 = imagensS1->begin();
    imgS1_2++;

    for(std::vector<C1_T>::iterator it = resultado->begin(); it != resultado->end(); ++it){
        it->tamanho = *tam;
        it->overlap = *over;
        for(int i = 0; i < nOrientacoes; i++){
#ifdef CUDAON
            cv::Mat aux  = imgS1->imgFiltrada[i];
#else
            cv::Mat aux  = imgS1->imgFiltrada[i];
            cv::Mat aux2 = imgS1_2->imgFiltrada[i];
#endif

            // Encontra o máximo ponto a ponto entre as duas imagens da faixa
            it->imgMaxBand[i] = cv::max(aux, aux2);
            it->orientation[i] = imgS1->orientation[i];

            // Utiliza a opeação Dilatação
            aux = it->imgMaxBand[i];
            cv::Mat element = cv::getStructuringElement( cv::MORPH_RECT,
                                                         cv::Size( *tam, *tam ),
                                                         cv::Point( floor(*tam/2), floor(*tam/2)));
            cv::dilate(aux, aux2, element);

            // Amostragem dos pontos
            int rows = (aux.rows / *over);
            int coluns = (aux.cols / *over);
            cv::resize(aux, it->imgMaxBand[i], cv::Size(coluns, rows), 0, 0, cv::INTER_NEAREST);
        }
        imgS1++;
        imgS1_2++;
        tam++;
        over++;
    }
}
