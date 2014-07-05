#include "c1pathdiccreator.h"

C1pathDicCreator::C1pathDicCreator(QObject *parent) :
    QThread(parent)
{
}

/**
 * @brief C1pathDicCreator::C1pathDicCreator
 * @param imagensParaAmostra
 * @param tamanhos
 * @param nAmostras
 * @param clusterizacao Quando verdadeiro, retira uma amostragem maior e cria o vocabulario usando o centro encontrados pelo kmeans como patchs
 * @param parent
 */
C1pathDicCreator::C1pathDicCreator(std::vector<C1_T> *imagensParaAmostra, std::vector<int> *tamanhos,
                                   std::vector<int> *nAmostras, bool clusterizacao, QObject *parent): QThread(parent)
{
    this->imagensParaAmostra = imagensParaAmostra;
    this->tamanhos = tamanhos;
    this->nAmostras = nAmostras;
    this->patchs = NULL;
    clusterizar = clusterizacao;
}

/**
 * @brief C1pathDicCreator::run
 *
 * Realiza a amostragem de patch da camada C1.
 */
void C1pathDicCreator::run(){
    if(clusterizar){
        /// @todo Implementar
    } else {
        int numTotalPats = 0;
        for(std::vector<int>::iterator it = nAmostras->begin(); it != nAmostras->end(); ++it)
            numTotalPats += *it;

        delete(patchs);
        patchs = new std::vector<patchC1>;
        patchs->resize(numTotalPats);
        std::vector<patchC1>::iterator pat = patchs->begin();
        std::vector<int>::iterator nAms = nAmostras->begin();
        for(std::vector<int>::iterator i = tamanhos->begin();  i != tamanhos->end(); ++i){
            int nPatPorIMG = (int)(*nAms/imagensParaAmostra->size());
            for(std::vector<C1_T>::iterator j = imagensParaAmostra->begin(); j != imagensParaAmostra->end(); ++j){
                for(int k = 0; k < nPatPorIMG; k++){
                    int rows = j->imgMaxBand[0].rows;
                    int col = j->imgMaxBand[0].cols;
                    if(rows-1 > *i && col-1 > *i){
                        int x = fabs((int)rand() % (col - ((*i)+1)));
                        int y = fabs((int)rand() % (rows - ((*i)+1)));
                        // Percorre as orientaçoes
                        for(int l = 0; l < nOrientacoesC2; l++){
                            cv::Rect roi(x, y, *i, *i);
                            cv::Mat auxiliar(j->imgMaxBand[l]);
                            cv::Mat crop(auxiliar, roi);
#ifdef CUDAON
                            pat->patch[l].upload(crop.clone());
#else
                            pat->patch[l] = crop.clone();
#endif
                        }
                    } else {
                        //std::cout << "Indexing Error, no good =(\n";
                    }

                    pat++;
                }
            }
            nAms++;
        }
    }
}


void C1pathDicCreator::salvaPatchesArquivo(QString file){
    cv::FileStorage fs(file.toUtf8().data(), cv::FileStorage::WRITE);
    if(this->patchs != NULL){
        fs << "nPatchs" << (int) this->patchs->size();
        fs << "nOrients" << (int) nOrientacoesC2;
        int i = 0;
        QString str;
        for(std::vector<patchC1>::iterator it = this->patchs->begin(); it != this->patchs->end(); ++it){
            for(int j = 0; j < nOrientacoesC2; j++){
                str = QString().sprintf("img_%d_orient_%d",i, j);
                fs << str.toUtf8().data() << ((cv::Mat)it->patch[j]);
            }
            i++;
        }
    }
    fs.release();
}

void C1pathDicCreator::loadPatchs(QString file){
    cv::FileStorage fs(file.toUtf8().data(), cv::FileStorage::READ);
    int nPatchs = (int)fs["nPatchs"];
    int nOrients = (int)fs["nOrients"];
    if(nOrients == nOrientacoesC2){
        QString str;
        delete(this->patchs);
        this->patchs = new std::vector<patchC1>;
        this->patchs->resize(nPatchs);
        int i = 0;
#ifdef  CUDAON
        cv::Mat auxCPU;
#endif
        for(std::vector<patchC1>::iterator it = this->patchs->begin(); it != this->patchs->end(); ++it){
            for(int j = 0; j < nOrientacoesC2; j++){
                str = QString().sprintf("img_%d_orient_%d",i, j);
#ifdef CUDAON
                fs[str.toUtf8().data()]  >> auxCPU;
                it->patch[j].upload(auxCPU);
#else
                fs[str.toUtf8().data()]  >> it->patch[j];
#endif
            }
            i++;
        }
    }
    fs.release();
}

void C1pathDicCreator::clusteriza(){
    cv::Mat amostra[4];
    amostra[0] = cv::Mat::zeros(1, 16, CV_32F);
    amostra[1] = cv::Mat::zeros(1, 64, CV_32F);
    amostra[2] = cv::Mat::zeros(1, 144, CV_32F);
    amostra[3] = cv::Mat::zeros(1, 256, CV_32F);
    for(std::vector<patchC1>::iterator it = patchs->begin(); it < patchs->end(); ++it){
        if(it->patch[0].rows == 4){
            cv::vconcat(amostra[0], it->patch[0].reshape(0, 1),amostra[0]);
        } else if(it->patch[0].rows == 8){
            cv::vconcat(amostra[1], it->patch[0].reshape(0, 1),amostra[1]);
        } else if(it->patch[0].rows == 12){
            cv::vconcat(amostra[2], it->patch[0].reshape(0, 1),amostra[2]);
        } else if(it->patch[0].rows == 16){
            cv::vconcat(amostra[3], it->patch[0].reshape(0, 1),amostra[3]);
        }
    }
    cv::TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 10000, 0.0001);
    cv::Mat centros[4];
    cv::Mat labels;
    cv::kmeans(amostra[0], KVOC, labels, cv::TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 10000, 0.0001), 5, cv::KMEANS_PP_CENTERS, centros[0]);
    cv::kmeans(amostra[1], KVOC, labels, cv::TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 10000, 0.0001), 5, cv::KMEANS_PP_CENTERS, centros[1]);
    cv::kmeans(amostra[2], KVOC, labels, cv::TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 10000, 0.0001), 5, cv::KMEANS_PP_CENTERS, centros[2]);
    cv::kmeans(amostra[3], KVOC, labels, cv::TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 10000, 0.0001), 5, cv::KMEANS_PP_CENTERS, centros[3]);
    patchs->resize(KVOC*4);
    std::vector<patchC1>::iterator iterator = patchs->begin();
    for(int i = 0; i < KVOC; i++){
        iterator->patch[0] = centros[0].row(i).reshape(0, 4);
        iterator++;
    }
    for(int i = 0; i < KVOC; i++){
        iterator->patch[0] = centros[1].row(i).reshape(0, 8);
        iterator++;
    }
    for(int i = 0; i < KVOC; i++){
        iterator->patch[0] = centros[2].row(i).reshape(0, 12);
        iterator++;
    }
    for(int i = 0; i < KVOC; i++){
        iterator->patch[0] = centros[3].row(i).reshape(0, 16);
        iterator++;
    }

}

void C1pathDicCreator::setPatchs(std::vector<patchC1> *pats){
    this->patchs = pats;
}

std::vector<patchC1>* C1pathDicCreator::getPatchs(){
    return this->patchs;
}
