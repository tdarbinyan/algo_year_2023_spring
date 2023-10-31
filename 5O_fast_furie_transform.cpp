#include <iostream>
#include <cmath>
#include <complex>
#include <vector>
#include <algorithm>

class Furie {
    int fsize_;
    int ssize_;
    std::vector<int64_t> first_;
    std::vector<int64_t> second_;
    std::vector<int64_t> ans_;

    void FFT(std::vector<std::complex<double>>& coeffs, bool inv) {
        int64_t sz = coeffs.size();
        if (sz == 1) {
            return;
        }

        std::vector<std::complex<double>> even(sz / 2), odd(sz / 2);

        for (int i = 0, j = 0; i < sz; i += 2, ++j) {
            even[j] = coeffs[i];
            odd[j] = coeffs[i + 1];
        }

        FFT(even, inv);
        FFT(odd, inv);

        double angle = (2 * acos(-1) / sz) * (inv ? -1 : 1);
        std::complex<double> sol(1), soln(cos(angle), sin(angle));

        for (int i = 0; i < sz / 2; ++i) {

            coeffs[i] = even[i] + sol * odd[i];
            coeffs[i + sz / 2] = even[i] - sol * odd[i];

            if (inv) {
                coeffs[i] /= 2;
                coeffs[i + sz / 2] /= 2;
            }
            sol *= soln;
        }
    }



public:
    void Input() {
        std::cin >> fsize_;
        ++fsize_;
        first_.resize(fsize_);
        for (int i = 0; i < fsize_; ++i) {
            std::cin >> first_[i];
        }

        std::cin >> ssize_;
        ++ssize_;
        second_.resize(ssize_);
        for (int i = 0; i < ssize_; ++i) {
            std::cin >> second_[i];
        }

        if ((fsize_ == 1 && first_[0] == 0) || (ssize_ == 1 && second_[0] == 0)) {
            return;
        }

        std::reverse(first_.begin(), first_.end());
        std::reverse(second_.begin(), second_.end());

        Multiply();
    }

    void Multiply() {
        size_t sz = 1;
        while (sz < std::max(first_.size(), second_.size())) {
            sz *= 2;
        }
        sz *= 2;

        std::vector<std::complex<double>> fur1(sz, 0), fur2(sz, 0);
        for (int64_t i = 0; i < first_.size(); ++i) {
            fur1[i] = std::complex<double>(first_[i], 0);
        }

        for (int64_t i = 0; i < second_.size(); ++i) {
            fur2[i] = std::complex<double>(second_[i], 0);
        }

        FFT(fur1, false), FFT(fur2, false);

        for (size_t i = 0; i < sz; ++i) {
            fur1[i] *= fur2[i];
        }

        FFT(fur1, true);

        ans_.resize(sz);
        for (size_t i = 0; i < sz; ++i) {
            ans_[i] = round(fur1[i].real());
        }
    }

    void Normalize() {
        while (!ans_.empty()) {
            if (ans_[ans_.size() - 1] == 0) {
                ans_.pop_back();
            }
            else {
                break;
            }
        }
    }

    void OutputAnswer() {
        if (ans_.empty()) {
            std::cout << 0 << ' ' << 0;
            return;
        }

        std::cout << ans_.size() - 1;

        for (int64_t i = ans_.size() - 1; i >= 0; --i) {
            std::cout << ' ' << ans_[i];
        }
    }
};




int main() {
    Furie f;

    f.Input();

    f.Multiply();

    f.Normalize();

    f.OutputAnswer();

    return 0;
}
