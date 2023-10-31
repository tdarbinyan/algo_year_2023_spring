#include <iostream>
#include <queue>
#include <utility>
#include <vector>

class Graph {
 private:
  size_t eng_num_, chin_num_;
  size_t vert_num_;
  std::vector<std::vector<size_t>> adjacency_list_;
  std::vector<std::vector<size_t>> residual_list_;

 public:
  bool BFS(size_t start, size_t finish, std::vector<size_t>& parent) {
    std::vector<bool> visited(vert_num_, false);
    std::queue<size_t> queue;
    queue.push(start);
    visited[start] = true;
    parent[start] = -1;

    while (!queue.empty()) {
      size_t from_vert = queue.front();
      queue.pop();

      for (size_t to_vert = 0; to_vert < vert_num_; ++to_vert) {
        if (!visited[to_vert] && residual_list_[from_vert][to_vert] > 0) {
          visited[to_vert] = true;
          parent[to_vert] = from_vert;
          queue.push(to_vert);
        }
      }
    }

    return visited[finish];
  }

  size_t FordFulkerson() {
    size_t start = 0;
    size_t finish = vert_num_ - 1;
    size_t max_flow = 0;
    std::vector<size_t> parent(vert_num_);

    while (BFS(start, finish, parent)) {
      size_t path_flow = 0;
      std::pair<size_t, size_t> edge = {0, 0};
      for (size_t to_vert = finish; to_vert != start;
           to_vert = parent[to_vert]) {
        size_t from_vert = parent[to_vert];

        if (to_vert != finish) {
          edge.second == 0 ? edge.second = to_vert : edge.first = to_vert;
        }

        if (to_vert == finish ||
            path_flow > residual_list_[from_vert][to_vert]) {
          path_flow = residual_list_[from_vert][to_vert];
        }
      }

      for (size_t to_vert = finish; to_vert != start;
           to_vert = parent[to_vert]) {
        size_t from_vert = parent[to_vert];

        residual_list_[from_vert][to_vert] -= path_flow;
        residual_list_[to_vert][from_vert] += path_flow;
      }

      max_flow += path_flow;
    }

    return max_flow;
  }

  void InputTheGraph() {
    std::cin >> eng_num_ >> chin_num_;

    vert_num_ = eng_num_ + chin_num_ + 2;

    adjacency_list_.assign(vert_num_, std::vector<size_t>(vert_num_, 0));
    residual_list_.assign(vert_num_, std::vector<size_t>(vert_num_, 0));

    for (size_t i = 1; i <= eng_num_; ++i) {
      size_t vert;
      for (size_t j = 0; j <= chin_num_; ++j) {
        std::cin >> vert;

        if (vert == 0) {
          break;
        }
        vert += eng_num_;
        adjacency_list_[i][vert] = 1;
        residual_list_[i][vert] = 1;

        adjacency_list_[vert][vert_num_ - 1] = 1;
        residual_list_[vert][vert_num_ - 1] = 1;
      }
      adjacency_list_[0][i] = 1;
      residual_list_[0][i] = 1;
    }
  }

  void OutputRoutes() {
    for (size_t i = 1; i <= eng_num_; ++i) {
      if (adjacency_list_[0][i] > 0) {
        for (size_t j = eng_num_ + 1; j <= eng_num_ + chin_num_; ++j) {
          if (adjacency_list_[i][j] > 0 && residual_list_[i][j] == 0) {
            std::cout << i << " " << j - eng_num_ << std::endl;
          }
        }
      }
    }
  }
};

int main() {
  Graph graph;

  graph.InputTheGraph();

  std::cout << graph.FordFulkerson() << std::endl;

  graph.OutputRoutes();

  return 0;
}
