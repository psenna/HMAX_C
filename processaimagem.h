#ifndef PROCESSAIMAGEM_H
#define PROCESSAIMAGEM_H

#include <QThread>
#include "HMax_Class/s1th.h"
#include "HMax_Class/c1th.h"
#include "HMax_Class/c1pathdiccreator.h"
#include "HMax_Class/c2th.h"

class ProcessaImagem : public QThread
{
    Q_OBJECT
public:
    explicit ProcessaImagem(QObject *parent = 0);

signals:

public slots:

};

#endif // PROCESSAIMAGEM_H
