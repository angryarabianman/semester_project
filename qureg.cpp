#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "qureg.h"


QuReg::QuReg(): reg_size(0) {}

QuReg::QuReg(ull size) {
    reg_size = size;
    State.resize(1ULL<<reg_size);
    srand(time(NULL));
}

QuReg::QuReg(const QuReg & old) {
    reg_size = old.reg_size;
    State = old.State;
}

std::complex<double> QuReg::GetProb(ull state) const {
    if (state >= 1ULL << reg_size) {
        throw -1;
    }
    return State[state];
}

void QuReg::Norm() {
    double b = 0;
    for (const auto &s : State) {
        b += pow(s.real(), 2) + pow(s.imag(), 2);
    }
    b = pow(b, -0.5);
    for (auto &s : State) {
        s = std::complex<double>(s.real() * b, s.imag() * b);
    }
}

int QuReg::Size() const {
    return reg_size;
}

ull QuReg::DecMeasure() {
    int done = 0, DecVal = -1, i = 0; // -1 error
    double rand1, a = 0, b = 0;
    rand1 = rand()/(double)RAND_MAX;
    for (auto &s : State) {
        if (!done ){
            b += pow(s.real(), 2) + pow(s.imag(), 2);
            if (b > rand1 && rand1 > a) {
                for (unsigned long long int j = 0; j < pow(2, reg_size) ; j++) {
                    s = std::complex<double>(0,0);
                }
                s = std::complex<double>(1,0);
                DecVal = i;
                done = 1;
            }
            a += pow(s.real(), 2) + pow(s.imag(), 2);
        }
        i++;
    }
    return DecVal;
}

void QuReg::Dump(int verbose) const {
    for (int i = 0 ; i < (1 << reg_size) ; i++) {
        if (verbose || fabs(State[i].real()) > eps
            || fabs(State[i].imag()) > eps) {
                 << State[i].real() << " + i" << State[i].imag()
                 << endl << flush;
        }
    }
}

void QuReg::SetState(std::vector<std::complex<double>>& new_state) {
    for (int i = 0 ; i < (1 << reg_size) ; i++) {
        State[i] = std::complex<double>(new_state[i].real(), new_state[i].imag());
    }
}

void QuReg::SetAverage(ull number) {
    if (number >= 1ULL << reg_size) {
    } else {
        double prob = pow(number, -0.5);
        for (unsigned long long int i = 0 ; i <= number ; i++) {
            State[i] = std::complex<double>(prob, 0);
        }
    }
}
