#include "bof.h"

Bof::Bof(QObject *parent) :
    QThread(parent)
{
}

Bof::Bof(QString nomeImg, cv::Mat *vocabulario, int classe){
    this->nomeImg = nomeImg;
    this->vocabulario = vocabulario;
    this->classe = classe;
}

cv::Mat Bof::getHistograma(){
    return this->histograma;
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
#ifdef FREAK_ON
    cv::FREAK descritor;
#else
    cv::ORB descritor;
#endif
    descritor.compute(img, pontosDeInteresse, descritores);

    // Classificar os pontos de interesse em uma palavra visual.
    histograma = cv::Mat().zeros(1, vocabulario->rows, CV_32F);

    for(int i = 0; i < descritores.rows; i++){
        cv::Mat aux = descritores.row(i).clone();
        cv::matchTemplate(*vocabulario, aux, aux, CV_TM_SQDIFF);
        cv::Point loc;
        cv::minMaxLoc(aux, NULL, NULL, &loc, NULL);
        histograma.at<float>(0,loc.y) += 1;
    }

    // Normalizando o histograma
    for(int i = 0; i < histograma.cols; i++){
        histograma.at<float>(0, i) /= descritores.rows;
    }
}

cv::Mat Bof::extraiCaract(){
    cv::Mat img = cv::imread(this->nomeImg.toUtf8().data(), 1);
    cv::cvtColor(img, img, CV_BGR2GRAY);

    // Encontrar pontos de interesse.
    std::vector<cv::KeyPoint> pontosDeInteresse;
#ifdef GOODDETECTOR
    cv::GoodFeaturesToTrackDetector detector;
#elif  DENSEDETECTOR
    cv::DenseFeatureDetector detector;
#endif
    detector.detect(img, pontosDeInteresse);

    // Descrever esses pontos.
    cv::Mat descritores;
#ifdef FREAK_ON
    cv::FREAK descritor;
#else
    cv::ORB descritor;
#endif
    descritor.compute(img, pontosDeInteresse, descritores);
    return descritores;
}

void Bof::saveVoc(){
    cv::FileStorage fs("VocabularioBOF.yml", cv::FileStorage::WRITE);
    if(this->vocabulario != NULL){
        fs << "Vocabulario" << (cv::Mat) *vocabulario;
    }
    fs.release();
}

cv::Mat Bof::loadVoc(QString nomeArq){
    cv::FileStorage fs(nomeArq.toUtf8().data(), cv::FileStorage::READ);
    cv::Mat voc;
    fs["Vocabulario"] >> voc;
    return voc;
}
