/**
 * Executa a camada C1 do modelo HMAX
 *
 * Cada faixa contem duas imagens
 *
 * Pedro Senna
 * Ciências da computação
 * Universidade Federal de Itajubá - UNIFEI
 *
 */

#ifndef C1TH_H
#define C1TH_H

#include "baseStructs.h"
#include <QThread>
#include <vector>

class C1th : public QThread
{
    Q_OBJECT
public:
    explicit C1th(QObject *parent = 0);
    explicit C1th(std::vector<int>* tamanho, std::vector<int>* overlap, std::vector<S1_T>* imagensS1, QObject *parent = 0);

    std::vector<C1_T>* resultado;

private:
    std::vector<S1_T>* imagensS1;
    std::vector<int>* tamanho;
    std::vector<int>* overlap;

    void run();

};

#endif // C1TH_H
