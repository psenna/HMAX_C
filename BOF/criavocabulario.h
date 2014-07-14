#ifndef CRIAVOCABULARIO_H
#define CRIAVOCABULARIO_H

#include <QThread>

class criaVocabulario : public QThread
{
    Q_OBJECT
public:
    explicit criaVocabulario(QObject *parent = 0);

signals:

public slots:

};

#endif // CRIAVOCABULARIO_H
