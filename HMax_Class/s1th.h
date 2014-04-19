#ifndef S1TH_H
#define S1TH_H

#include <QThread>

class S1Th : public QThread
{
    Q_OBJECT
public:
    explicit S1Th(QObject *parent = 0);

signals:

public slots:

};

#endif // S1TH_H
