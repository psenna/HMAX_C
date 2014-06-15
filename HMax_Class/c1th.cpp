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

        // Cria o elemento da dilatação
#ifdef  CUDAON
        cv::gpu::GpuMat element;
        element.upload(cv::getStructuringElement( cv::MORPH_RECT,
                                                  cv::Size( *tam, *tam ),
                                                  cv::Point( floor(*tam/2), floor(*tam/2))));
#else
        cv::Mat element = cv::getStructuringElement( cv::MORPH_RECT,
                                                     cv::Size( *tam, *tam ),
                                                     cv::Point( floor(*tam/2), floor(*tam/2)));
#endif

        for(int i = 0; i < nOrientacoes; i++){
#ifdef CUDAON
            cv::gpu::GpuMat aux  = imgS1->imgFiltrada[i];
            cv::gpu::GpuMat aux2 = imgS1_2->imgFiltrada[i];
#else
            cv::Mat aux  = imgS1->imgFiltrada[i];
            cv::Mat aux2 = imgS1_2->imgFiltrada[i];
#endif

            // Encontra o máximo ponto a ponto entre as duas imagens da faixa
#ifdef CUDAON
            cv::gpu::max(aux, aux2,it->imgMaxBand[i]);
            it->orientation[i] = imgS1->orientation[i];
#else
            it->imgMaxBand[i] = cv::max(aux, aux2);
            it->orientation[i] = imgS1->orientation[i];
#endif


            // Utiliza a opeação Dilatação
#ifdef CUDAON
            aux = it->imgMaxBand[i];
            cv::gpu::dilate(aux, aux2, element);

#else
            aux = it->imgMaxBand[i];
            cv::dilate(aux, aux2, element);

#endif

            // Amostragem dos pontos
            int rows = (aux.rows / *over);
            int coluns = (aux.cols / *over);
#ifdef CUDAON
            cv::gpu::resize(aux, it->imgMaxBand[i], cv::Size(coluns, rows), 0, 0, cv::INTER_NEAREST);
#else
            cv::resize(aux, it->imgMaxBand[i], cv::Size(coluns, rows), 0, 0, cv::INTER_NEAREST);
#endif
        }
        imgS1+=2;
        imgS1_2+=2;
        tam++;
        over++;
    }
}
