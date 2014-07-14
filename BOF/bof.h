#ifndef BOF_H
#define BOF_H

#include "../HMax_Class/baseStructs.h"
#include <QThread>

class Bof : public QThread
{
    Q_OBJECT
public:
    explicit Bof(QObject *parent = 0);
    Bof(QString nomeImg, cv::Mat* vocabulario);
    void roda();
    cv::Mat extraiCaract();
    void saveVoc();
    cv::Mat loadVoc(QString nomeArq);

private:
    cv::Mat* vocabulario;
    QString nomeImg;

};

#endif // BOF_H
