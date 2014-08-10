#ifndef BOF_H
#define BOF_H

#include "../HMax_Class/baseStructs.h"
#include <QThread>

class Bof : public QThread
{
    Q_OBJECT
public:
    explicit Bof(QObject *parent = 0);
    Bof(QString nomeImg, cv::Mat* vocabulario, int classe = 0);
    void roda();
    cv::Mat extraiCaract();
    void saveVoc();
    cv::Mat loadVoc(QString nomeArq);
    cv::Mat getHistograma();
    cv::Mat Pyr(int level);
    std::vector<cv::Mat> pyrAux;
    std::vector<cv::Mat> pyrAuxMax;
    int classe;

private:
    cv::Mat* vocabulario;
    QString nomeImg;
    cv::Mat histograma;

};

#endif // BOF_H
