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

#include <QThread>

class C1th : public QThread
{
    Q_OBJECT
public:
    explicit C1th(QObject *parent = 0);

signals:

public slots:

};

#endif // C1TH_H
