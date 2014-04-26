#include "c1pathdiccreator.h"

C1pathDicCreator::C1pathDicCreator(QObject *parent) :
    QThread(parent)
{
}

/**
 * @brief C1pathDicCreator::C1pathDicCreator
 * @param imagensParaAmostra
 * Imagens usadas para extrair as amostras.
 * @param tamanhos
 * Tamanho do lado dos quadrados das amostras.
 * @param nAmostras
 * Quantidade de amostras para cada tamanho.
 * @param parent
 */
C1pathDicCreator::C1pathDicCreator(std::vector<C1_T> *imagensParaAmostra, std::vector<int> *tamanhos,
                                   std::vector<int> *nAmostras, QObject *parent): QThread(parent)
{
    this->imagensParaAmostra = imagensParaAmostra;
    this->tamanhos = tamanhos;
    this->nAmostras = nAmostras;
    this->patchs = NULL;
}

void C1pathDicCreator::run(){

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

void C1pathDicCreator::setPatchs(std::vector<patchC1> *pats){
    this->patchs = pats;
}
