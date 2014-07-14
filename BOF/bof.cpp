#include "bof.h"

Bof::Bof(QObject *parent) :
    QThread(parent)
{
}

Bof::Bof(QString nomeImg, cv::Mat *vocabulario){
    this->nomeImg = nomeImg;
    this->vocabulario = vocabulario;
}

void Bof::roda(){
    cv::Mat img = cv::imread(this->nomeImg.toUtf8().data(), 1);
    cv::cvtColor(img, img, CV_BGR2GRAY);

    // Encontrar pontos de interesse.
    std::vector<cv::KeyPoint> pontosDeInteresse;
    cv::DenseFeatureDetector detector;
    detector.detect(img, pontosDeInteresse);

    // Descrever esses pontos.
    cv::Mat descritores;
    cv::FREAK descritor;
    descritor.compute(img, pontosDeInteresse, descritores);

    // Classificar os pontos de interesse em uma palavra visual.
    cv::Mat histograma = cv::Mat().zeros(1, vocabulario->rows, CV_32F);

    for(int i = 0; i < descritores.rows; i++){
        cv::Mat aux = descritores.row(i);
        cv::matchTemplate(*vocabulario, aux, aux, CV_TM_SQDIFF);
        cv::Point loc;
        cv::minMaxLoc(aux, NULL, NULL, &loc, NULL);
        histograma.at<float>(0,loc.x)++;
    }

    // Normalizando o histograma
    cv::Scalar total = cv::sum(histograma);
    cv::divide(total[0], histograma, histograma);

}
