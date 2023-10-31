#include <iostream>
#include <unordered_set>
#include <vector>

class Hash {
  std::vector<int64_t> powers_;
  const uint64_t kMod_ = 2e32 - 1;
  const int64_t kAlphShift_ = 96;
  const int64_t kPrime_ = 29;
  const int64_t kMaxNum = 2000000;
  std::unordered_set<int64_t> hash_;
  std::string pattern_;
  std::string text_;
  int64_t ans_ = 0;

 public:
  void Input() {
    std::cin >> pattern_ >> text_;
    if (pattern_.size() > text_.size()) {
      std::cout << 0;
      return;
    }

    ReservePower();

    InitiateAlgo();
  }

  void ReservePower() {
    powers_.resize(kMaxNum, 1);

    for (int64_t i = 1; i < kMaxNum; ++i) {
      powers_[i] = (kPrime_ * powers_[i - 1]) % kMod_;
    }
  }

  std::unordered_set<int64_t> Shift(std::string pattern) {
    int64_t hash;
    std::unordered_set<int64_t> temp;

    for (int64_t i = 0; i < pattern.size(); ++i) {
      if (i == 0) {
        hash = (pattern[i] - kAlphShift_) * powers_[pattern.size() - 1] % kMod_;
      } else {
        hash = (hash +
                (pattern[i] - kAlphShift_) * powers_[pattern.size() - i - 1]) %
               kMod_;
      }
    }

    int64_t shifted_hash = hash;
    temp.insert(shifted_hash);

    pattern = pattern + pattern;
    for (int64_t i = (pattern.size() / 2); i < pattern.size(); ++i) {
      shifted_hash =
          ((shifted_hash - (pattern[i - (pattern.length() / 2)] - kAlphShift_) *
                               powers_[(pattern.length() / 2) - 1]) *
               kPrime_ +
           (pattern[i] - kAlphShift_)) %
          kMod_;
      temp.insert(shifted_hash);
    }

    return temp;
  }

  void InitiateAlgo() {
    hash_ = Shift(pattern_);

    int64_t hash = 0;
    for (int64_t i = 0; i < pattern_.size(); ++i) {
      hash =
          (hash + (text_[i] - kAlphShift_) * powers_[pattern_.size() - i - 1]) %
          kMod_;
    }

    for (int64_t i = pattern_.size(); i <= text_.size(); ++i) {
      if (hash_.count(hash)) {
        ++ans_;
      }
      hash = ((hash - (text_[i - pattern_.size()] - kAlphShift_) *
                          powers_[pattern_.size() - 1]) *
                  kPrime_ +
              (text_[i] - kAlphShift_)) %
             kMod_;
    }
    if (hash_.count(hash)) {
      ++ans_;
    }

    OutputAnswer();
  }

  void OutputAnswer() { std::cout << ans_; }
};

int main() {
  std::ios::sync_with_stdio(0);
  std::cin.tie(0);
  std::cout.tie(0);

  Hash hash;
  hash.Input();

  return 0;
}
