#ifndef SHOR_H
#define SHOR_H

#include <bits/stdc++.h>
#include <QObject>
#include "qureg.h"
#include "util.h"

class shor : public QObject
{

    Q_OBJECT

public:
    shor(QObject *parent = nullptr);
    std::vector<int> calc(ull n);
signals:
    void mySignal();
    void signalTries(int x);
};

#endif // SHOR_H
