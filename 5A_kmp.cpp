#include <iostream>
#include <vector>

std::vector<int> Prefix(std::string pattern) {
  std::vector<int> pi(pattern.size());
  pi[0] = 0;
  for (int i = 1, j = 0; i < pattern.size(); i++) {
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

void Find(std::string text, std::string pattern) {
  std::vector<int> pi = Prefix(pattern);
  for (int i = 0, j = 0; i < text.size(); i++) {
    while (j > 0 && text[i] != pattern[j]) {
      j = pi[j - 1];
    }
    if (text[i] == pattern[j]) {
      j++;
    }
    if (j == pattern.size()) {
      std::cout << i - pattern.size() + 1 << '\n';
      j = pi[j - 1];
    }
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  std::string text, pattern;
  std::cin >> text >> pattern;
  Find(text, pattern);
  return 0;
}
