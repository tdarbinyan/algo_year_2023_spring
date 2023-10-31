#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

struct Edge {
  int64_t from;
  int64_t to;
  int64_t capacity;
  int64_t flow = 0;

  Edge() = default;

  Edge(int64_t vertex1, int64_t vertex2, int64_t capacity)
      : from(vertex1), to(vertex2), capacity(capacity){};
};

class Graph {
 private:
  int64_t vert_num_;
  size_t edge_num_;
  int64_t max_flow_ = 0;
  std::vector<Edge> edges_;
  std::vector<std::vector<int64_t>> graph_;
  std::vector<int64_t> ptr_;
  std::vector<int64_t> distance_;

 public:
  Graph() = default;

  bool Bfs() {
    std::queue<int64_t> queue;
    queue.push(0);
    distance_.assign(distance_.size(), -1);
    distance_[0] = 0;
    while (!queue.empty() && distance_[vert_num_ - 1] == -1) {
      int64_t from_vert = queue.front();
      queue.pop();
      for (auto& to_vert : graph_[from_vert]) {
        if (distance_[edges_[to_vert].to] == -1 &&
            edges_[to_vert].flow < edges_[to_vert].capacity) {
          queue.push(edges_[to_vert].to);
          distance_[edges_[to_vert].to] = distance_[from_vert] + 1;
        }
      }
    }
    return (distance_[vert_num_ - 1] != -1);
  }

  int64_t Dfs(int64_t vert, int64_t flow) {
    if (flow != 0) {
      if (vert == vert_num_ - 1) {
        return flow;
      }
      for (size_t i = ptr_[vert]; i < graph_[vert].size(); ++i) {
        if (edges_[graph_[vert][i]].capacity >= 0 &&
            distance_[edges_[graph_[vert][i]].to] == distance_[vert] + 1) {
          int64_t dfs_path =
              Dfs(edges_[graph_[vert][i]].to,
                  std::min(flow, edges_[graph_[vert][i]].capacity -
                                     edges_[graph_[vert][i]].flow));
          if (dfs_path != 0) {
            edges_[graph_[vert][i]].flow += dfs_path;
            edges_[graph_[vert][i] ^ 1].flow -= dfs_path;
            return dfs_path;
          }
        }
        ++ptr_[vert];
      }
    }
    return 0;
  }

  void Run() {
    std::cin >> vert_num_ >> edge_num_;

    graph_.resize(vert_num_);
    ptr_.resize(vert_num_);
    distance_.resize(vert_num_);

    for (size_t i = 0; i < edge_num_; ++i) {
      int64_t first;
      int64_t second;
      int64_t capacity;

      std::cin >> first >> second >> capacity;

      graph_[--first].push_back(edges_.size());
      graph_[--second].push_back(edges_.size() + 1);
      edges_.push_back(Edge(first, second, capacity));
      edges_.push_back(Edge(second, first, 0));
    }

    while (Bfs()) {
      ptr_.assign(graph_.size(), 0);
      while (int64_t flow = Dfs(0, INT64_MAX)) {
        max_flow_ += flow;
      }
    }

    std::cout << max_flow_ << '\n';
    for (size_t i = 0; i < edges_.size(); i += 2) {
      std::cout << edges_[i].flow << '\n';
    }
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  Graph graph;

  graph.Run();

  return 0;
}
