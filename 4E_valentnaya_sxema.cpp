#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <utility>
#include <vector>

std::map<char, int> valent{{'.', 0}, {'H', 1}, {'O', 2}, {'N', 3}, {'C', 4}};

class Graph {
 private:
  int vert_size_;
  int m_size_;
  int n_size_;
  int answer_ = 0;
  int left_half_ = 0;
  int right_half_ = 0;
  bool empty_checker_ = true;
  std::vector<std::vector<char>> field_;
  std::vector<std::vector<std::pair<int, int>>> residual_list_;
  std::pair<int, int> no_edge_ = {-1, -1};

 public:
  bool BFS(int start, int finish, std::vector<int>& parent) {
    std::vector<bool> visited(vert_size_, false);
    std::queue<int> queue;
    queue.push(start);
    visited[start] = true;
    parent[start] = -1;

    while (!queue.empty()) {
      int from_vert = queue.front();
      queue.pop();

      for (auto& edge : residual_list_[from_vert]) {
        int to_vert = edge.first;
        int capacity = edge.second;

        if (!visited[to_vert] && capacity > 0) {
          visited[to_vert] = true;
          parent[to_vert] = from_vert;
          queue.push(to_vert);
        }
      }
    }

    return visited[finish];
  }

  std::pair<int, int>& GetEdge(int from, int to) {
    for (auto& edge : residual_list_[from]) {
      if (edge.first == to) {
        return edge;
      }
    }
    return no_edge_;
  }

  int FordFulkerson() {
    int start = 0;
    int finish = vert_size_ - 1;
    int max_flow = 0;
    std::vector<int> parent(vert_size_);
    while (BFS(start, finish, parent)) {
      int path_flow = INT32_MAX;
      for (int to_vert = finish; to_vert != start; to_vert = parent[to_vert]) {
        int from_vert = parent[to_vert];
        if (GetEdge(from_vert, to_vert).first != -1) {
          path_flow = std::min(GetEdge(from_vert, to_vert).second, path_flow);
        }
      }
      for (int to_vert = finish; to_vert != start; to_vert = parent[to_vert]) {
        int from_vert = parent[to_vert];
        GetEdge(from_vert, to_vert).second -= path_flow;

        if (GetEdge(to_vert, from_vert).first != -1) {
          GetEdge(to_vert, from_vert).second += path_flow;
        } else {
          residual_list_[to_vert].push_back({from_vert, path_flow});
        }
      }
      max_flow += path_flow;
    }
    return max_flow;
  }

  void EdgeBuilder(int idx, int jdx) {
    empty_checker_ = false;

    int index = idx * n_size_ + jdx + 1;

    if ((jdx > 0) && (field_[idx][jdx - 1] != '.')) {
      (idx + jdx) % 2 == 1 ? residual_list_[index].push_back({index - 1, 1})
                           : residual_list_[index - 1].push_back({index, 1});
    }

    if ((idx > 0) && (field_[idx - 1][jdx] != '.')) {
      (idx + jdx) % 2 == 1
          ? residual_list_[index].push_back({index - n_size_, 1})
          : residual_list_[index - n_size_].push_back({index, 1});
    }

    (idx + jdx) % 2 == 1
        ? residual_list_[0].push_back({index, valent[field_[idx][jdx]]})
        : residual_list_[index].push_back(
              {vert_size_ - 1, valent[field_[idx][jdx]]});

    (idx + jdx) % 2 == 1 ? left_half_ += valent[field_[idx][jdx]]
                         : right_half_ += valent[field_[idx][jdx]];
  }

  void InputTheGraph() {
    std::cin >> m_size_ >> n_size_;

    vert_size_ = m_size_ * n_size_ + 2;

    residual_list_.resize(vert_size_);
    field_.assign(m_size_, std::vector<char>(n_size_));

    for (int i = 0; i < m_size_; ++i) {
      for (int j = 0; j < n_size_; ++j) {
        std::cin >> field_[i][j];
        if (field_[i][j] == '.') {
          continue;
        }

        EdgeBuilder(i, j);
      }
    }
  }

  void CoutTheAnswer() {
    answer_ = FordFulkerson();

    if (!empty_checker_ && answer_ == left_half_ && answer_ == right_half_) {
      std::cout << "Valid";
    } else {
      std::cout << "Invalid";
    }
  }
};

int main() {
  Graph graph;

  graph.InputTheGraph();

  graph.CoutTheAnswer();

  return 0;
}
