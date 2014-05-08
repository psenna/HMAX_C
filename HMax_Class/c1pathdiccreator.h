/**
 * Cria o vocabulario de patchs C1 usados na camada S2.
 *
 * Pedro Senna
 * Ciências da computação
 * Universidade Federal de Itajubá - UNIFEI
 *
 */

#ifndef C1PATHDICCREATOR_H
#define C1PATHDICCREATOR_H

#include <QThread>
#include <set>
#include "baseStructs.h"

class C1pathDicCreator : public QThread
{
    Q_OBJECT
private:
    std::vector<C1_T> *imagensParaAmostra;
    std::vector<patchC1> *patchs;
    std::vector<int> *tamanhos;
    std::vector<int> *nAmostras;
    bool clusterizar;

public:
    explicit C1pathDicCreator(QObject *parent = 0);
    explicit C1pathDicCreator(std::vector<C1_T> *imagensParaAmostra, std::vector<int> *tamanhos,
                              std::vector<int> *nAmostras, bool clusterizacao = false, QObject *parent = 0); 

    std::vector<patchC1>* getPatchs();
    void setPatchs(std::vector<patchC1>* pats);

    void salvaPatchesArquivo(QString file);
    void loadPatchs(QString file);
    void clusteriza(int k);

    void run();

};

#endif // C1PATHDICCREATOR_H
