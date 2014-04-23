#ifndef C2TH_H
#define C2TH_H

#include <QThread>

class C2th : public QThread
{
    Q_OBJECT
public:
    explicit C2th(QObject *parent = 0);

signals:

public slots:

};

#endif // C2TH_H
