#include <iostream>
#include <queue>
#include <string>
#include <utility>
#include <vector>

class Graph {
 private:
  int16_t vert_size_;
  int16_t m_size_;
  int16_t n_size_;
  int16_t answer_ = 0;
  int16_t star_counter_ = 0;
  int16_t first_weight_;
  int16_t second_weight_;
  std::vector<std::string> field_;
  std::vector<std::vector<std::pair<int16_t, int16_t>>> residual_list_;

 public:
  bool BFS(int16_t start, int16_t finish, std::vector<int16_t>& parent) {
    std::vector<bool> visited(vert_size_, false);
    std::queue<int16_t> queue;
    queue.push(start);
    visited[start] = true;
    parent[start] = -1;

    while (!queue.empty()) {
      int16_t from_vert = queue.front();
      queue.pop();

      for (auto& edge : residual_list_[from_vert]) {
        int16_t to_vert = edge.first;
        int16_t capacity = edge.second;

        if (!visited[to_vert] && capacity > 0) {
          visited[to_vert] = true;
          parent[to_vert] = from_vert;
          queue.push(to_vert);
        }
      }
    }

    return visited[finish];
  }

  int16_t FordFulkerson() {
    int16_t start = 0;
    int16_t finish = vert_size_ - 1;
    int16_t max_flow = 0;
    std::vector<int16_t> parent(vert_size_);

    while (BFS(start, finish, parent)) {
      int16_t path_flow = INT16_MAX;
      for (int16_t to_vert = finish; to_vert != start;
           to_vert = parent[to_vert]) {
        int16_t from_vert = parent[to_vert];

        for (auto& edge : residual_list_[from_vert]) {
          if (edge.first == to_vert) {
            path_flow = std::min(path_flow, edge.second);
            edge.second -= path_flow;
            break;
          }
        }

        bool edge_exists = false;
        for (auto& edge : residual_list_[to_vert]) {
          if (edge.first == from_vert) {
            edge.second += path_flow;
            edge_exists = true;
            break;
          }
        }

        if (!edge_exists) {
          residual_list_[to_vert].push_back({from_vert, path_flow});
        }
      }
      max_flow += path_flow;
    }

    return max_flow;
  }

  void InputTheGraph() {
    std::cin >> m_size_ >> n_size_ >> first_weight_ >> second_weight_;

    vert_size_ = m_size_ * n_size_ + 2;

    residual_list_.resize(vert_size_);
    field_.assign(m_size_, std::string(n_size_, 'x'));

    for (int16_t i = 0; i < m_size_; ++i) {
      for (int16_t j = 0; j < n_size_; ++j) {
        std::cin >> field_[i][j];
        if (field_[i][j] == '.') {
          continue;
        }

        ++star_counter_;

        int16_t index = i * n_size_ + j + 1;
        if ((j > 0) && (field_[i][j - 1] == '*')) {
          (i + j) % 2 == 1 ? residual_list_[index].push_back({index - 1, 1})
                           : residual_list_[index - 1].push_back({index, 1});
          ;
        }

        if ((i > 0) && (field_[i - 1][j] == '*')) {
          (i + j) % 2 == 1
              ? residual_list_[index].push_back({index - n_size_, 1})
              : residual_list_[index - n_size_].push_back({index, 1});
          ;
        }

        (i + j) % 2 == 1 ? residual_list_[0].push_back({index, 1})
                         : residual_list_[index].push_back({vert_size_ - 1, 1});
      }
    }
  }

  void CoutTheAnswer() {
    answer_ = FordFulkerson();

    if (first_weight_ < 2 * second_weight_) {
      std::cout << answer_ * first_weight_ +
                       (star_counter_ - 2 * answer_) * second_weight_;
    } else {
      std::cout << star_counter_ * second_weight_;
    }
  }
};

int main() {
  Graph graph;

  graph.InputTheGraph();

  graph.CoutTheAnswer();

  return 0;
}
