#include "mainwindow.h"
#include <QApplication>
#include <QFileDialog>
#include "HMax_Class/s1th.h"
#include "HMax_Class/c1th.h"
#include "HMax_Class/c1pathdiccreator.h"
#include "HMax_Class/c2th.h"

#define CUDAON

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();

}
