#include "qureg.h"
#include <bits/stdc++.h>

#ifndef UTIL_H
#define UTIL_H

ull TestPrime(unsigned long long int n);

ull TestPrimePower(unsigned long long int n);

ull GCD(unsigned long long int a, unsigned long long int b);

ull RegSize(unsigned long long int a);

ull GetQ(unsigned long long int n);

ull modexp(unsigned long long int x, unsigned long long int a, unsigned long long int n);

ull denominator(double c, unsigned long long int qmax);

ull max(unsigned long long int a, unsigned long long int b);

void DFT(QuReg * reg, unsigned long long int q);

#endif
