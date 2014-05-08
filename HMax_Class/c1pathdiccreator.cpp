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
                    if(rows > *i && col > *i){
                        int x = fabs((int)rand() % (col - ((*i)+1)));
                        int y = fabs((int)rand() % (rows - ((*i)+1)));
                        // Percorre as orientaçoes
                        for(int l = 0; l < nOrientacoes; l++){
                            cv::Rect roi(x, y, *i, *i);
                            cv::Mat crop(j->imgMaxBand[l], roi);
                            pat->patch[l] = crop.clone();
                        }
                    } else {
                        std::cout << "Indexing Error, no good =(\n";
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
        fs << "nOrients" << (int) nOrientacoes;
        int i = 0;
        QString str;
        for(std::vector<patchC1>::iterator it = this->patchs->begin(); it != this->patchs->end(); ++it){
            for(int j = 0; j < nOrientacoes; j++){
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
    if(nOrients == nOrientacoes){
        QString str;
        delete(this->patchs);
        this->patchs = new std::vector<patchC1>;
        this->patchs->resize(nPatchs);
        int i = 0;
        for(std::vector<patchC1>::iterator it = this->patchs->begin(); it != this->patchs->end(); ++it){
            for(int j = 0; j < nOrientacoes; j++){
                str = QString().sprintf("img_%d_orient_%d",i, j);
                fs[str.toUtf8().data()]  >> it->patch[j];
            }
            i++;
        }
    }
    fs.release();
}

void C1pathDicCreator::clusteriza(int k){
    if(this->patchs != NULL){
        int dimensoes = 30*15;
        int count[4] = {0,0,0,0};
        cv::Mat toCluster[4];
        toCluster[i] = cv::Mat();

        for(std::vector<patchC1>::iterator it = this->patchs->begin(); it != this->patchs->end(); ++it){


        }
    }
}

void C1pathDicCreator::setPatchs(std::vector<patchC1> *pats){
    this->patchs = pats;
}

std::vector<patchC1>* C1pathDicCreator::getPatchs(){
    return this->patchs;
}
