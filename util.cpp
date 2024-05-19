#include <iostream>
#include <math.h>
#include "qureg.h"

using namespace std;

ull TestPrime(ull n) {
    for (ull i = 2 ; i * i <= n ; i++) if (!(n%i)) {
        return 0;
    }
    return 1;
}

ull TestPrimePower(ull n) {
    ull i = 2, j = 0;
    while (i * i <= n && j == 0) {
        if((n % i) == 0) {
            j = i;
        }
        i++;
    }
    for (ull i = 2 ; i <= (floor(log(n) / log(j)) + 1) ; i++) {
        if(pow(j , i) == n) {
            return 1;
        }
    }
    return 0;
}

ull GCD(ull a, ull b) {
    ull d;
    if (b == 0) return -1;

    while (a % b != 0) {
        d = a % b;
        a = b;
        b = d;
    }
    return b;
}

ull RegSize(ull a) {
    ull size = 0;
    while(a != 0) {
        a = a>>1;
        size++;
    }
    return(size);
}


ull GetQ(ull n) {
    ull power = 8; //256 is the smallest q ever is.
    while (pow(2,power) < pow(n,2)) {
        power = power + 1;
    }
    return 1ULL << power;
}

ull modexp(ull x, ull a, ull n) {
    ull value = 1, tmp = x % n;
    while (a > 0) {
        if (a & 1) {
            value = (value * tmp) % n;
        }
        tmp = tmp * tmp % n;
        a = a>>1;
    }
    return value;
}

ull denominator(double c, ull qmax) {
    double y = c, z;
    ull q0 = 0, q1 = 1, q2 = 0;
    while (1) {
        z = y - floor(y);
        if (z < 0.5 / pow(qmax,2)) {
            return q1;
        }
        if (z != 0) {
            y = 1 / z;
        } else {
            return q1;
        }
        q2 = (ull)floor(y) * q1 + q0;
        if (q2 >= qmax) {
            return q1;
        }
        q0 = q1;
        q1 = q2;
    }
}

ull max(ull a, ull b) {
    if (a > b) {
        return a;
    }
    return b;
}

void DFT(QuReg * reg, ull q) {

    std::vector<std::complex<double>> init(q);

    for (ull a = 0; a < q ; a++) {
        if ((pow(reg->GetProb(a).real(),2) +
             pow(reg->GetProb(a).imag(),2)) > eps) {
            for (ull c = 0 ; c < q ; c++) {
                auto tmpcomp = std::complex<double>(pow(q,-.5) * cos(2*M_PI*a*c/q), pow(q,-.5) * sin(2*M_PI*a*c/q));
                init[c] = init[c] + (reg->GetProb(a) * tmpcomp);
            }
        }
        if (a % 300 == 0) {
            cout << "Making progress in Fourier transform, "
                 << 100*((double)a / (double)(q - 1)) << "% done!"
                 << endl << flush;
        }
    }
    reg->SetState(init);
    reg->Norm();
}
