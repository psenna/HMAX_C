#include "processaclasses.h"

ProcessaClasses::ProcessaClasses(QObject *parent) :
    QThread(parent)
{
    iniciaParametros();
}

void ProcessaClasses::iniciaParametros(){

    for(int i = 0; i < nOrientacoes; i++){
        orientacaoS1.push_back(PI*i/(nOrientacoes+1));
    }

    S1Th s1(cv::Mat(), &tamanhosS1, &lambdaS1, &sigmaS1, &gamaS1, &orientacaoS1, NULL);
    s1.criaFiltro();
    filtrosGaborS1.insert(filtrosGaborS1.end(), s1.filters->begin(), s1.filters->end());
    delete(s1.filters);

}

void ProcessaClasses::criaVocabulario(QString imagem){
    ProcessaImagem p1(imagem, 0, &filtrosGaborS1, &tamanhosS1, &lambdaS1, &sigmaS1, &gamaS1, &orientacaoS1, &tamanhoC1, &overlapC1, NULL);
    p1.roda();
    this->patsC1.insert(patsC1.end(), p1.patsC1->begin(), p1.patsC1->end());
}

void ProcessaClasses::run(){
    int numImagens = 0;
    for(std::vector<classeImagem>::iterator it = this->classesImagens.begin(); it != this->classesImagens.end(); ++it){
        numImagens += it->numImgs;
    }
    emit numImagensAseremProcessadas(numImagens);

    for(std::vector<classeImagem>::iterator it = this->classesImagens.begin(); it != this->classesImagens.end(); ++it){
        srand(time(NULL));
        QStringList nameFilter("*.jpg");
        QDir directory(it->caminho);
        QStringList imageFiles = directory.entryList(nameFilter);

        for(int i = 0; imageFiles.size() && i < it->numImgs; i++){
            int imgSorteada = rand() % imageFiles.size();
            QString arquivo = imageFiles.at(imgSorteada);
            imageFiles.removeAt(imgSorteada);
            arquivo = it->caminho + "/" + arquivo;
            ProcessaImagem *img = new ProcessaImagem(arquivo, it->id, &filtrosGaborS1, &tamanhosS1,
                                                     &lambdaS1, &sigmaS1, &gamaS1, &orientacaoS1,
                                                     &tamanhoC1, &overlapC1, &patsC1);
            this->threadsImagens.push_back(img);
            img->roda();
            emit acabouProcessarImagem();
        }
    }

    emit acabouDeProcessarAsImagens();
}

void ProcessaClasses::acabouThread(){
    mutex.lock();
    nThreadRodando--;
    mutex.unlock();
    emit acabouProcessarImagem();
}


