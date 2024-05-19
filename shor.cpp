#include "shor.h"

#include <QDebug>

shor::shor(QObject *parent)
{
    Q_UNUSED(parent);
}


std::vector<int> shor::calc(ull n){
    srand(time(0));
    std::vector<int> twos;
    if (TestPrime(n)) {
        twos.push_back(n);
        return twos;
    }
    if (n % 2 == 0) {
        twos.push_back(2);
        twos.push_back(n / 2);
        return twos;
    }
    if (n == 1) {
        return twos;
    }
    if (TestPrimePower(n)) {
        for (int i=2;;i++) if (n % i == 0) {
            twos.push_back(i);
            twos.push_back(n / i);
            break;
        }
        return twos;
    }
    ull q = GetQ(n), x = 0;
    x = 1+ (ull)((n-1)*(double)rand()/(double)RAND_MAX);
    while (GCD(n,x) != 1 || x == 1) {
        x = 1 + (ull)((n-1)*(double)rand()/(double)RAND_MAX);
    }
    QuReg * reg1 = new QuReg(RegSize(q) - 1);
    std::vector<ull> modex(q);
    std::vector<std::complex<double>> collapse(q);
    std::vector<std::complex<double>> mdx (1<<RegSize(n));
    QuReg *reg2 = new QuReg(RegSize(n));

    ull value, den;
    int factor;
    bool done = false;
    unsigned int tries = 0;

    auto work = [&]() -> std::vector<int>{
        emit signalTries(tries + 1);
        qDebug() << tries;
        reg1->SetAverage(q - 1);

        auto tmp = std::complex<double>(1,0);

        for (ull i = 0 ; i < q ; i++) {
            auto tmpval = modexp(x,i,n);
            modex[i] = tmpval;
            mdx[tmpval] += tmp;
        }

        reg2->SetState(mdx);

        reg2->Norm();

        value = reg2->DecMeasure();

        for (ull i = 0 ; i < q ; i++) {
            if (modex[i] == value) {
                collapse[i] = std::complex<double>(1,0);
            } else {
                collapse[i] = std::complex<double>(0,0);
            }
        }

        reg1->SetState(collapse);
        reg1->Norm();
        DFT(reg1, q);
        double m = reg1->DecMeasure();
        done = true;

        if (m == 0) {
            done = false;
        }

        if (m == -1) {
            done = false;
        }

        if (done) {
            auto c = (double)m  / (double)q;

            den = denominator(c, q);

            if (den % 2 == 1 && 2 * den < q ){
                cout << "\tOdd candidate for r found, expanding by 2\n";
                den = 2 * den;
            }

            if (den % 2 == 1)  {
                done = false;
            } else {
                auto e = modexp(x, den / 2, n);
                ull a = (e + 1) % n;
                ull b = (e + n - 1) % n;
                factor = max(GCD(n,a),GCD(n,b));
            }
        }

        if ((factor == n || factor == 1) && done == 1) {
            done = false;
        }

        if (factor != 0 && done == 1) {
            return {(int)factor, (int)n / (int)factor};
        } else if (done == 1) {
            done = false;
        }
        tries++;
        return {};
    };
    while (!done) {
        auto v = work();
        if ((int)v.size() > 1) {
            return v;
        }
        if (tries >= 100) {
            break;
        }
    }
    delete reg1;
    delete reg2;
    return {-1};
}
