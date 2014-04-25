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
                                   std::vector<int> *nAmostras, QObject *parent)
{
    this->imagensParaAmostra = imagensParaAmostra;
    this->tamanhos = tamanhos;
    this->nAmostras = nAmostras;
    this->patchs = NULL;
}

void C1pathDicCreator::run(){

}


// http://docs.opencv.org/modules/core/doc/xml_yaml_persistence.html
// Pode resolver com menos trabalho!
void C1pathDicCreator::salvaPatchesArquivo(QString file){
    if(this->patchs != NULL){

    }
}

void C1pathDicCreator::loadPatchs(QString file){

}
