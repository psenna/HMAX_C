#ifndef S2TH_H
#define S2TH_H

#include <QThread>

class S2th : public QThread
{
    Q_OBJECT
public:
    explicit S2th(QObject *parent = 0);

signals:

public slots:

};

#endif // S2TH_H
