#include <bits/stdc++.h>

#ifndef QUREG_H
#define QUREG_H

using namespace std;
using ull = unsigned long long;
using ll = long long;
const double eps = 1e-14;

class QuReg {
public:
    QuReg(ull size);

    QuReg();

    QuReg(const QuReg &);

    unsigned long long int DecMeasure();

    void Dump(int verbose) const;

    void SetState(std::vector<std::complex<double>>& new_state);

    void SetAverage(ull number);

    void Norm();

    std::complex<double> GetProb(ull state) const;

    int Size() const;

private:
    ull reg_size;
    std::vector<std::complex<double>> State;
};

#endif
