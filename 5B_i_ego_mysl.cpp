#include <iostream>
#include <vector>

std::vector<int> Prefix(std::string pattern) {
  if (pattern.empty()) {
    return std::vector<int>();
  }
  std::vector<int> pi(pattern.size());
  pi[0] = 0;
  for (int i = 1, j = 0; i < pattern.size(); ++i) {
    while (j > 0 && pattern[i] != pattern[j]) {
      j = pi[j - 1];
    }
    if (pattern[i] == pattern[j]) {
      j++;
    }
    pi[i] = j;
  }
  return pi;
}

std::string SufPrefCompress(std::vector<std::string>& words) {
  std::string ans = words[0];
  for (int i = 1; i < words.size(); i++) {
    std::vector<int> pi =
        Prefix(words[i] + '#' +
               ans.substr(std::max(0, static_cast<int>(ans.size()) -
                                          static_cast<int>(words[i].size()))));
    if (!pi.empty()) {
      int longest_presuf = pi.back();
      while (longest_presuf > words[i].size()) {
        longest_presuf = pi[longest_presuf - 1];
      }
      ans += words[i].substr(longest_presuf);
    }
  }
  return ans;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  int num;
  std::cin >> num;
  std::vector<std::string> words(num);

  for (int i = 0; i < num; ++i) {
    std::cin >> words[i];
  }

  std::cout << SufPrefCompress(words) << '\n';
  return 0;
}
