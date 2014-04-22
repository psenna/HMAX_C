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

void C1th::run(){
    this->resultado = new std::vector<C1_T>;
    resultado->resize((int)floor(imagensS1->size()/2));

    std::vector<int>::iterator tam = tamanho->begin();
    std::vector<int>::iterator over = overlap->begin();
    std::vector<S1_T>::iterator imgS1 = imagensS1->begin();

    for(std::vector<C1_T>::iterator it = resultado->begin(); it != resultado->end(); ++it){
        cv::Mat aux  = imgS1->imgFiltrada;
        imgS1++;
        cv::Mat aux2 = imgS1->imgFiltrada;

        // Encontra o máximo ponto a ponto entre as duas imagens da faixa
        it->imgMaxBand = cv::max(aux, aux2);
        it->tamanho = *tam;
        it->overlap = *over;
        it->orientation = imgS1->orientation;

        // Utiliza a opeação Dilatação
        aux = it->imgMaxBand;
        cv::Mat element = cv::getStructuringElement( cv::MORPH_RECT,
                                           cv::Size( *tam, *tam ),
                                           cv::Point( floor(*tam/2), floor(*tam/2)) );
        cv::dilate( aux, it->imgMaxBand, element);

        //! @todo Realizar a amostragem
        imgS1++;
        tam++;
        over++;
        tam++;
      }

}
