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
        if(nOrientacoesC2 == 1){
            for(int i = 1; i < nOrientacoes; i++){
                imgS1->imgFiltrada[0] = cv::max(imgS1->imgFiltrada[0], imgS1->imgFiltrada[i]);
                imgS1_2->imgFiltrada[0] = cv::max(imgS1_2->imgFiltrada[0], imgS1_2->imgFiltrada[i]);
            }
        }
        for(int i = 0; i < nOrientacoesC2; i++){
            cv::Mat aux  = imgS1->imgFiltrada[i];
            cv::Mat aux2 = imgS1_2->imgFiltrada[i];

            // Encontra o máximo ponto a ponto entre as duas imagens da faixa
            it->imgMaxBand[i] = cv::max(aux, aux2);

            // Utiliza a opeação Dilatação
            aux = it->imgMaxBand[i];
            cv::Mat element = cv::getStructuringElement( cv::MORPH_RECT,
                                                         cv::Size( *tam, *tam ),
                                                         cv::Point( floor(*tam/2), floor(*tam/2)));
            cv::dilate(aux, aux2, element);

            // Amostragem dos pontos
            int rows = (aux.rows / *over);
            int coluns = (aux.cols / *over);
            cv::resize(aux, aux, cv::Size(coluns, rows), 0, 0, cv::INTER_NEAREST);
#ifdef  CUDAON
            it->imgMaxBand[i].upload(aux);
#else
            it->imgMaxBand[i] = aux;
#endif
        }
        imgS1+=2;
        imgS1_2+=2;
        tam++;
        over++;
    }
}
