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

    cv::Mat roi;
    int px = img.cols/ (1<<NivelPyr);
    int py = img.rows/ (1<<NivelPyr);
    std::vector<cv::KeyPoint> pontosDeInteresse;
    cv::DenseFeatureDetector detector;
    cv::Mat descritores;
    this->pyrAux.clear();
    this->pyrAuxMax.clear();
    for(int i = 0; i < (1<<NivelPyr); i++){
        for(int j = 0; j < (1<<NivelPyr); j++){
            // Encontrar pontos de interesse.
            roi = img(cv::Rect(i*px, j*py, px, py));
            detector.detect(roi, pontosDeInteresse);

            // Descrever esses pontos.
            if(tipoDescritor == ORB){
                cv::ORB descritor;
                descritor.compute(img, pontosDeInteresse, descritores);
            }else if(tipoDescritor == FREAK){
                cv::FREAK descritor;
                descritor.compute(img, pontosDeInteresse, descritores);
            }else if(tipoDescritor == SURF){
                cv::SURF descritor;
                descritor.compute(img, pontosDeInteresse, descritores);
            }else{
                cv::SiftDescriptorExtractor descritor;
                descritor.compute(img, pontosDeInteresse, descritores);
            }

            // Classificar os pontos de interesse em uma palavra visual.
            cv::Mat AuxP = cv::Mat().zeros(1, vocabulario->rows, CV_32F);
#ifdef BOFMAX
            cv::Mat AuxMaxP = cv::Mat().zeros(vocabulario->rows, 1, CV_32F);
            AuxMaxP.setTo(DBL_MAX);
#endif

            for(int k = 0; k < descritores.rows; k++){
                cv::Mat aux = descritores.row(k).clone();
                cv::matchTemplate(*vocabulario, aux, aux, CV_TM_SQDIFF);
#ifdef BOFMAX
                AuxMaxP = cv::min(AuxMaxP, aux);
#endif
                cv::Point loc;
                cv::minMaxLoc(aux, NULL, NULL, &loc, NULL);
                AuxP.at<float>(0,loc.y) += 1;
            }
#ifdef BOFMAX
            this->pyrAuxMax.push_back(AuxMaxP);
#endif
            this->pyrAux.push_back(AuxP);
        }
    }

    cv::Mat Contador;
    cv::Mat Hist2;
    for(int i = NivelPyr; i >= 0  && this->pyrAuxMax.size() > 0; i--){
        Contador = this->pyrAuxMax[0];
        for(int j = 1; j < this->pyrAuxMax.size(); j++){
            Contador = cv::min(Contador, this->pyrAuxMax[j]);
        }
        for(int j = 0; j < Contador.rows; j++){
            Contador.at<float>(j,0) *= (ATENUACAO<<i);
        }
        if(i == NivelPyr){
            Hist2 = Contador;
        } else {
            cv::vconcat(Hist2, Contador, Hist2);
        }

        // Reducao no nivel da piramide
        int ref = 0;
        int desloc = 1<<i;
        for(int j = 0; j < floor(desloc/2); j++){
            for(int k = 0; k < floor(desloc/2); k++){
                this->pyrAuxMax[ref] = cv::max(this->pyrAuxMax[(j *2 * desloc) + k*2], this->pyrAuxMax[(j *2 * desloc) + (k*2)+1]);
                this->pyrAuxMax[ref] = cv::max(this->pyrAuxMax[(((j*2) + 1) * desloc) + k*2], this->pyrAuxMax[ref]);
                this->pyrAuxMax[ref] = cv::max(this->pyrAuxMax[(((j*2)+1) * desloc) + (k*2)+1], this->pyrAuxMax[ref]);
                ref++;
            }
        }
        this->pyrAuxMax.resize(this->pyrAuxMax.size()/4);
    }
    histograma = cv::Mat().zeros(1,Hist2.rows,CV_32F);
    for(int i = 0; i < histograma.cols; i++){
        histograma.at<float>(0,i) = (float)exp(-1.0 * Hist2.at<float>(i, 0)/AJUSTEGAUSSIANABOF) * AJUSTELINEARBOF;
    }

}

cv::Mat Bof::extraiCaract(){
    cv::Mat img = cv::imread(this->nomeImg.toUtf8().data(), 1);
    cv::cvtColor(img, img, CV_BGR2GRAY);

    // Encontrar pontos de interesse.
    std::vector<cv::KeyPoint> pontosDeInteresse;
#ifdef GOODDETECTOR
    cv::GoodFeaturesToTrackDetector detector;
#else
    cv::DenseFeatureDetector detector;
#endif
    detector.detect(img, pontosDeInteresse);

    // Descrever esses pontos.
    cv::Mat descritores;
    if(tipoDescritor == ORB){
        cv::ORB descritor;
        descritor.compute(img, pontosDeInteresse, descritores);
    }else if(tipoDescritor == FREAK){
        cv::FREAK descritor;
        descritor.compute(img, pontosDeInteresse, descritores);
    }else if(tipoDescritor == SURF){
        cv::SURF descritor;
        descritor.compute(img, pontosDeInteresse, descritores);
    }else{
        cv::SiftDescriptorExtractor descritor;
        descritor.compute(img, pontosDeInteresse, descritores);
    }
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
