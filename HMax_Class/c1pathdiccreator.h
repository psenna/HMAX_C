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
#include "baseStructs.h"

class C1pathDicCreator : public QThread
{
    Q_OBJECT
public:
    explicit C1pathDicCreator(QObject *parent = 0);
    explicit C1pathDicCreator(std::vector<C1_T> *imagensParaAmostra, std::vector<int> *tamanhos,
                              std::vector<int> *nAmostras, QObject *parent = 0);
    std::vector<patchC1>* getPatchs();

    void salvaPatchesArquivo(QString file);
    void loadPatchs(QString file);

private:
    std::vector<C1_T> *imagensParaAmostra;
    std::vector<patchC1> *patchs;
    std::vector<int> *tamanhos;
    std::vector<int> *nAmostras;

    void run();

};

#endif // C1PATHDICCREATOR_H
