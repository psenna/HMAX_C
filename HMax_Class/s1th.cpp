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
S1Th::S1Th(cv::Mat image, std::vector<int> *sizes, std::vector<float> *lambda, std::vector<float> *sigma,
           std::vector<float> *gama, std::vector<float> *orientation, std::vector<Filter_T> *filters, QObject *parent):
    QThread(parent)
{
#ifdef CUDAON
    this->image.upload(image);
#else
    this->image = image;   
#endif
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
void S1Th::roda(){
    this->gaborFilterResult = new std::vector<S1_T>;
<<<<<<< HEAD
    gaborFilterResult->resize(NUMBANDAS);
    std::vector<S1_T>::iterator result = gaborFilterResult->begin();
    if(image.rows > image.cols){
        int novaRow = (int)std::ceil(image.rows * MENORTAMIMG / image.cols );
#ifdef CUDAON
        cv::gpu::resize(image, image, cv::Size(MENORTAMIMG, novaRow), 0, 0, cv::INTER_CUBIC);
#else
        cv::resize(image, image, cv::Size(MENORTAMIMG, novaRow), 0, 0, cv::INTER_CUBIC);
#endif
    } else {
        int novaCol = (int)std::ceil(image.cols * MENORTAMIMG / image.rows );
#ifdef CUDAON
        cv::gpu::resize(image, image, cv::Size(novaCol, MENORTAMIMG), 0, 0, cv::INTER_CUBIC);
#else
        cv::resize(image, image, cv::Size(novaCol, MENORTAMIMG), 0, 0, cv::INTER_CUBIC);
#endif
    }

    for(int i = 0; i < NUMBANDAS; i++){
        std::vector<float>::iterator orient = orientation->begin();
        std::vector<Filter_T>::iterator filter = filters->begin();
        for(int j = 0; j < (int)orientation->size(); j++){
            result->orientation[j] = *orient;
#ifdef  CUDAON2
            cv::gpu::filter2D(image, result->imgFiltrada[j], CV_8U, *filter);
#else
            cv::filter2D(image, result->imgFiltrada[j], CV_8U, *filter);
#endif
            filter++;
            orient++;
        }
        result++;
#ifdef CUDAON
        cv::gpu::resize(image, image, cv::Size(0, 0), REDUCAOS1, REDUCAOS1, cv::INTER_CUBIC);
#else
        cv::resize(image, image, cv::Size(0, 0), REDUCAOS1, REDUCAOS1, cv::INTER_CUBIC);
#endif
=======
    // Caso os filtros tenham que ser criados.
    if(filters == NULL){
        // Nao e mais suportado, criar primeiro os filtros
        /*
        int quantidade = orientation->size() * sizes->size();
        gaborFilterResult->resize(sizes->size());

        filters = new std::vector<cv::Mat>;
        filters->resize(quantidade);

        std::vector<cv::Mat>::iterator filter = filters->begin();
        std::vector<S1_T>::iterator result = gaborFilterResult->begin();
        std::vector<int>::iterator tamanho = sizes->begin();
        std::vector<float>::iterator lamb = lambda->begin();
        std::vector<float>::iterator sig = sigma->begin();
        std::vector<float>::iterator gam = gama->begin();

        for(int i = 0; i < (int)sizes->size(); i++){
            std::vector<float>::iterator orient = orientation->begin();
            for(int j = 0; j < (int)orientation->size(); j++){
                *filter = cv::getGaborKernel(cv::Size(*tamanho, *tamanho), *sig, *orient, *lamb, *gam, 0, CV_32F);
                result->tamanho = *tamanho;
                result->orientation[j] = *orient;
                cv::filter2D(image, result->imgFiltrada[j], CV_32F, *filter);

                filter++;
                orient++;
            }
            lamb++;
            sig++;
            gam++;
            tamanho++;
            result++;
        }
    */
    } else {
        /// Quando os filtros ja foram calculados e usados em outras imagens.
        gaborFilterResult->resize(filters->size()/nOrientacoes);
        std::vector<cv::Mat>::iterator filter = filters->begin();
        std::vector<S1_T>::iterator result = gaborFilterResult->begin();
        std::vector<int>::iterator tamanho = sizes->begin();

        for(int i = 0; i < (int)sizes->size(); i++){
            std::vector<float>::iterator orient = orientation->begin();
            for(int j = 0; j < (int)orientation->size(); j++){
                result->tamanho = *tamanho;
                result->orientation[j] = *orient;

                cv::filter2D(image, result->imgFiltrada[j], CV_32F, *filter);

                filter++;
                orient++;
            }
            result++;
            tamanho++;
        }
>>>>>>> parent of a4f508c... C2 Cuda
    }
}

void S1Th::criaFiltro(){
    if(filters == NULL){
        int quantidade = orientation->size();
        filters = new std::vector<Filter_T>;
        filters->resize(quantidade);

        std::vector<Filter_T>::iterator filter = filters->begin();
<<<<<<< HEAD
        std::vector<float>::iterator orient = orientation->begin();
=======
        std::vector<int>::iterator tamanho = sizes->begin();
        std::vector<float>::iterator lamb = lambda->begin();
        std::vector<float>::iterator sig = sigma->begin();
        std::vector<float>::iterator gam = gama->begin();

        for(int i = 0; i < (int)sizes->size(); i++){
            std::vector<float>::iterator orient = orientation->begin();
            for(int j = 0; j < (int)orientation->size(); j++){
#ifdef CUDAON
        *filter.upload(cv::getGaborKernel(cv::Size(*tamanho, *tamanho), *sig, *orient, *lamb, *gam, 0, CV_32F));
#else
        *filter = cv::getGaborKernel(cv::Size(*tamanho, *tamanho), *sig, *orient, *lamb, *gam, 0, CV_32F);
#endif
>>>>>>> parent of a4f508c... C2 Cuda

        for(int j = 0; j < (int)orientation->size(); j++){
            *filter = cv::getGaborKernel(cv::Size(TAMANHOS1, TAMANHOS1), SIGMAS1, *orient, LAMBDAS1, GAMAS1, 0, CV_32F);
            filter++;
            orient++;
        }
    }
}
