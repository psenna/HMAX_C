#include "s1th.h"

S1Th::S1Th(QObject *parent) :
    QThread(parent)
{
}

/**
 * @brief Deve seguir a regra de funcionamento do metodo run.
 * @param parent
 * @param image
 * @param sizes
 * @param lambda
 * @param sigma
 * @param gama
 * @param orientation
 * @param filters
 */
S1Th::S1Th(cv::Mat image, std::vector<int> *sizes, std::vector<double> *lambda, std::vector<double> *sigma,
           std::vector<double> *gama, std::vector<double> *orientation, std::vector<cv::Mat> *filters, QObject *parent):
    QThread(parent)
{
    this->image = image;
    this->sizes = sizes;
    this->lambda = lambda;
    this->sigma = sigma;
    this->gama = gama;
    this->orientation = orientation;
    this->filters = filters;
}

/**
 * @brief S1Th::run
 *
 * Os vetores Tamanho e orientaçao sempre devem ser enviados, os outros parametros sao opicionais caso o
 * vetor de filtros seja enviado e obrigatorios caso o de filtro seja NULL.
 */
//! @todo Verificar se o temanho dos vetores sao iguais e emite alerta em caso negativo.
void S1Th::run(){
    this->gaborFilterResult = new std::vector<S1_T>;
    // Caso os filtros tenham que ser criados.
    if(filters == NULL){
        int quantidade = orientation->size() * sizes->size();
        gaborFilterResult->resize(sizes->size());

        filters = new std::vector<cv::Mat>;
        filters->resize(quantidade);

        std::vector<cv::Mat>::iterator filter = filters->begin();
        std::vector<S1_T>::iterator result = gaborFilterResult->begin();
        std::vector<int>::iterator tamanho = sizes->begin();

        for(int i = 0; i < (int)sizes->size(); i++){
            std::vector<double>::iterator orient = orientation->begin();
            std::vector<double>::iterator lamb = lambda->begin();
            std::vector<double>::iterator sig = sigma->begin();
            std::vector<double>::iterator gam = gama->begin();
            for(int j = 0; j < (int)orientation->size(); j++){
                *filter = cv::getGaborKernel(cv::Size(*tamanho, *tamanho), *sig, *orient, *lamb, *gam, 0, CV_64F);
                result->tamanho = *tamanho;
                result->orientation[j] = *orient;

                cv::filter2D(image, result->imgFiltrada[j], CV_64F, *filter);

                filter++;
                orient++;
                lamb++;
                sig++;
                gam++;
            }
            tamanho++;
            result++;
        }

    } else {
        /// Quando os filtros ja foram calculados e usados em outras imagens.
        gaborFilterResult->resize(filters->size()/nOrientacoes);
        std::vector<cv::Mat>::iterator filter = filters->begin();
        std::vector<S1_T>::iterator result = gaborFilterResult->begin();
        std::vector<int>::iterator tamanho = sizes->begin();

        for(int i = 0; i < (int)sizes->size(); i++){
            std::vector<double>::iterator orient = orientation->begin();
            for(int j = 0; j < (int)orientation->size(); j++){
                result->tamanho = *tamanho;
                result->orientation[j] = *orient;

                cv::filter2D(image, result->imgFiltrada[j], CV_64F, *filter);

                filter++;
                orient++;
            }
            result++;
            tamanho++;
        }
    }
}
