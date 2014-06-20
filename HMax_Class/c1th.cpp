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
#ifdef  CUDAON
        cv::gpu::GpuMat element;
        element.upload(cv::getStructuringElement( cv::MORPH_RECT,
                                                  cv::Size( *tam, *tam ),
                                                  cv::Point( floor(*tam/2), floor(*tam/2))));
#else
        cv::Mat element = cv::getStructuringElement( cv::MORPH_RECT,
<<<<<<< HEAD
                                                     cv::Size( TAMANHOC1, TAMANHOC1 ),
                                                     cv::Point( floor(TAMANHOC1/2), floor(TAMANHOC1/2)));

        for(int i = 0; i < nOrientacoes; i++){

            // Utiliza a opeação Dilatação
#ifdef CUDAON
            cv::gpu::dilate(imgS1->imgFiltrada[i], it->imgMaxBand[i], element);
=======
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
>>>>>>> parent of a4f508c... C2 Cuda

#else
            cv::dilate(imgS1->imgFiltrada[i], it->imgMaxBand[i], element);

#endif

            // Amostragem dos pontos
<<<<<<< HEAD
            int rows = ceil(it->imgMaxBand[i].rows / AMOSTRAGEMC1);
            int coluns = ceil(it->imgMaxBand[i].cols / AMOSTRAGEMC1);
#ifdef CUDAON
            cv::gpu::resize(it->imgMaxBand[i], it->imgMaxBand[i], cv::Size(coluns, rows), 0, 0, cv::INTER_NEAREST);
=======
            int rows = (aux.rows / *over);
            int coluns = (aux.cols / *over);
#ifdef CUDAON
            cv::gpu::resize(aux, it->imgMaxBand[i], cv::Size(coluns, rows), 0, 0, cv::INTER_NEAREST);
>>>>>>> parent of a4f508c... C2 Cuda
#else
            cv::resize(it->imgMaxBand[i], it->imgMaxBand[i], cv::Size(coluns, rows), 0, 0, cv::INTER_NEAREST);
#endif
        }
        imgS1++;
    }
}
