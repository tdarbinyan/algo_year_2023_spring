#include <iostream>
#include <queue>
#include <set>
#include <utility>
#include <vector>

class Graph {
 private:
  size_t answer_;
  size_t vert_num_, edge_num_;
  std::vector<std::vector<size_t>> adjacency_list_;
  std::vector<std::vector<size_t>> residual_list_;
  std::vector<std::vector<size_t>> order_;
  std::vector<size_t> adjacent_to_start_;
  std::vector<bool> visited_after_ff_;

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

  void BFSAfterFF(size_t start) {
    visited_after_ff_.assign(vert_num_, false);
    std::queue<size_t> queue;
    queue.push(start);
    adjacent_to_start_.push_back(start);
    visited_after_ff_[start] = true;

    while (!queue.empty()) {
      size_t from_vert = queue.front();
      queue.pop();

      for (size_t to_vert = 0; to_vert < vert_num_; ++to_vert) {
        if (!visited_after_ff_[to_vert] &&
            residual_list_[from_vert][to_vert] > 0) {
          visited_after_ff_[to_vert] = true;
          adjacent_to_start_.push_back(to_vert);
          queue.push(to_vert);
        }
      }
    }
  }

  void OutputCutEdges() {
    BFSAfterFF(0);

    std::set<size_t> cut_edges;

    for (size_t i = 0; i < adjacent_to_start_.size(); ++i) {
      for (size_t j = 0; j < vert_num_; ++j) {
        if (!visited_after_ff_[j] &&
            adjacency_list_[adjacent_to_start_[i]][j] > 0) {
          cut_edges.insert(order_[adjacent_to_start_[i]][j]);
        }
      }
    }

    std::cout << cut_edges.size() << " " << answer_ << std::endl;

    for (auto iter : cut_edges) {
      std::cout << iter << " ";
    }
  }

  // ok
  size_t FordFulkerson() {
    size_t start = 0;
    size_t finish = vert_num_ - 1;
    size_t max_flow = 0;
    std::vector<size_t> parent(vert_num_);

    while (BFS(start, finish, parent)) {
      size_t path_flow = 0;

      for (size_t to_vert = finish; to_vert != start;
           to_vert = parent[to_vert]) {
        size_t from_vert = parent[to_vert];

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
    answer_ = max_flow;

    return max_flow;
  }

  // ok
  void InputTheGraph() {
    std::cin >> vert_num_ >> edge_num_;

    adjacency_list_.assign(vert_num_, std::vector<size_t>(vert_num_, 0));
    residual_list_.assign(vert_num_, std::vector<size_t>(vert_num_, 0));
    order_.assign(vert_num_, std::vector<size_t>(vert_num_, 0));

    for (size_t i = 0; i < edge_num_; ++i) {
      size_t first;
      size_t second;
      size_t capacity;
      std::cin >> first >> second >> capacity;

      adjacency_list_[--first][--second] = capacity;
      adjacency_list_[second][first] = capacity;

      residual_list_[first][second] = capacity;
      residual_list_[second][first] = capacity;

      order_[first][second] = i + 1;
      order_[second][first] = i + 1;
    }
  }

  /* void OutputRoutes() {
       for (size_t i = 1; i <= vert_num_; ++i) {
           if (adjacency_list_[0][i] > 0) {
               for (size_t j = vert_num_ + 1; j <= vert_num_ + edge_num_; ++j) {
                   if (adjacency_list_[i][j] > 0 && residual_list_[i][j] == 0) {
                       std::cout << i << " " << j - vert_num_ << std::endl;
                   }
               }
           }
       }
   }*/
};

int main() {
  Graph graph;

  graph.InputTheGraph();

  graph.FordFulkerson();

  graph.OutputCutEdges();

  return 0;
}
