#include <algorithm>
#include <cmath>
#include <complex>
#include <iostream>
#include <string>
#include <vector>

namespace konst {
const int maxnum = 262144;
const double pi = acos(-1);

}  // namespace konst
class Furie {
  std::string str;
  std::vector<int> num_;
  std::complex<double> roots[konst::maxnum];
  int rvrs[konst::maxnum];
  int64_t ans_ = 0;

 public:
  void Input() {
    std::cin >> str;

    InitializeA();

    Multiply();

    OutputAnswer();
  }

  void InitializeA() {
    num_.resize(str.size());

    for (int i = 0; i < str.size(); i++) {
      num_[i] = (str[i] == '1' ? 1 : 0);
    }
  }

  void OutputAnswer() {
    for (int i = 0; i < num_.size(); i++) {
      if (num_[i] % 2 == 1) {
        ans_ += num_[i] / 2;
      }
    }
    std::cout << ans_ << '\n';
  }

  void FFT(std::vector<std::complex<double>>& vec) {
    int n = vec.size();
    for (int i = 0; i < n; ++i) {
      if (i < rvrs[i]) {
        swap(vec[i], vec[rvrs[i]]);
      }
    }

    for (int len = 1, stp = n / 2; len < n; len *= 2, stp /= 2) {
      for (int st = 0; st < n; st += 2 * len) {
        for (int it = 0, root = 0; it < len; ++it, root += stp) {
          std::complex<double> fval = vec[st + it];
          std::complex<double> sval = roots[root] * vec[st + len + it];

          vec[st + it] = fval + sval;
          vec[st + it + len] = fval - sval;
        }
      }
    }
  }

  void Multiply() {
    std::vector<std::complex<double>> polynome(num_.size());
    for (size_t i = 0; i < num_.size(); ++i) {
      polynome[i] = num_[i];
    }

    int n = 1, pow = 0;
    while (n < polynome.size() + polynome.size()) {
      n *= 2;
      ++pow;
    }

    while (polynome.size() < n) {
      polynome.push_back(0);
    }

    rvrs[0] = 0;
    for (int i = 1, high = -1; i < n; ++i) {
      high += !(i & (i - 1));
      rvrs[i] = rvrs[i ^ (1 << high)] | (1 << (pow - high - 1));
    }

    double angle = 2. * konst::pi / n;
    for (int i = 0; i < n; ++i) {
      roots[i] = std::complex<double>(cos(angle * i), sin(angle * i));
    }

    FFT(polynome);
    for (int i = 0; i < n; ++i) {
      polynome[i] *= polynome[i];
    }

    FFT(polynome);
    reverse(polynome.begin() + 1, polynome.end());

    for (std::complex<double>& it : polynome) {
      it /= n;
    }

    num_.clear();

    for (int i = 0; i < n; i++) {
      num_.push_back(round(polynome[i].real()));
    }
  }
};

int main() {
  Furie f;

  f.Input();
}
