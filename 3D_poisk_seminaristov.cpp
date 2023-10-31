#include <algorithm>
#include <iostream>
#include <vector>

class Heuristics {
 private:
  size_t size = 0;
  size_t total_weight = 0;
  std::vector<size_t> parent;
  std::vector<size_t> power;
  std::vector<std::pair<size_t, size_t>> ostov;
  std::vector<std::pair<size_t, std::pair<size_t, size_t>>> edges;

 public:
  Heuristics(size_t size) {
    this->parent.resize(size);
    this->power.resize(size);
    for (size_t i = 0; i < size; i++) {
      this->parent[i] = i;
      this->power[i] = 0;
    }
  }

  size_t Find(size_t& vert) {
    if (vert == this->parent[vert]) {
      return vert;
    }
    this->parent[vert] = Find(this->parent[vert]);
    return this->parent[vert];
  }

  void Unite(size_t& first, size_t& second) {
    first = Find(first);
    second = Find(second);
    if (first != second) {
      this->parent[first] = second;
    }
  }

  size_t KruskalReturnWeight(size_t requests) {
    std::sort(this->edges.begin(), this->edges.end());

    for (size_t i = 0; i < requests; ++i) {
      if (Find(this->edges[i].second.first) !=
          Find(this->edges[i].second.second)) {
        this->ostov.push_back(this->edges[i].second);
        total_weight += this->edges[i].first;
        Unite(this->edges[i].second.first, this->edges[i].second.second);
      }
    }
    return total_weight;
  }

  std::vector<std::pair<size_t, std::pair<size_t, size_t>>>& GetEdges() {
    return this->edges;
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  size_t size;
  std::cin >> size;

  Heuristics heur(size + 1);

  for (size_t i = 0; i < size; ++i) {
    size_t val;
    for (size_t j = 0; j <= i; ++j) {
      std::cin >> val;
    }
    for (size_t j = i + 1; j < size; ++j) {
      std::cin >> val;
      heur.GetEdges().push_back(std::make_pair(val, std::make_pair(i, j)));
    }
  }

  for (size_t i = 0; i < size; ++i) {
    size_t val;
    std::cin >> val;
    heur.GetEdges().push_back(std::make_pair(val, std::make_pair(i, size)));
  }

  std::cout << heur.KruskalReturnWeight(size * (size + 1) / 2) << "\n";

  return 0;
}
