#include <iostream>
#include <queue>
#include <vector>

class Graph {
 private:
  int vert_num_, edge_num_;
  std::vector<std::vector<int>> adjacency_list_;
  std::vector<std::vector<int>> residual_list_;

 public:
  bool BFS(int start, int finish, std::vector<int>& parent) {
    std::vector<bool> visited(vert_num_, false);
    std::queue<int> queue;
    queue.push(start);
    visited[start] = true;
    parent[start] = -1;

    while (!queue.empty()) {
      int from_vert = queue.front();
      queue.pop();

      for (int to_vert = 0; to_vert < vert_num_; ++to_vert) {
        if (!visited[to_vert] && residual_list_[from_vert][to_vert] > 0) {
          visited[to_vert] = true;
          parent[to_vert] = from_vert;
          queue.push(to_vert);
        }
      }
    }

    return visited[finish];
  }

  int FordFulkerson() {
    int start = 0;
    int finish = vert_num_ - 1;
    int max_flow = 0;
    std::vector<int> parent(vert_num_);

    while (BFS(start, finish, parent)) {
      int path_flow = 0;
      for (int to_vert = finish; to_vert != start; to_vert = parent[to_vert]) {
        int from_vert = parent[to_vert];

        if (to_vert == finish ||
            path_flow > residual_list_[from_vert][to_vert]) {
          path_flow = residual_list_[from_vert][to_vert];
        }
      }

      for (int to_vert = finish; to_vert != start; to_vert = parent[to_vert]) {
        int from_vert = parent[to_vert];

        residual_list_[from_vert][to_vert] -= path_flow;
        residual_list_[to_vert][from_vert] += path_flow;
      }

      max_flow += path_flow;
    }

    return max_flow;
  }

  void InputTheGraph() {
    std::cin >> vert_num_ >> edge_num_;

    adjacency_list_.assign(vert_num_, std::vector<int>(vert_num_, 0));
    residual_list_.assign(vert_num_, std::vector<int>(vert_num_, 0));

    for (int i = 0; i < edge_num_; ++i) {
      int first;
      int second;
      int capacity;
      std::cin >> first >> second >> capacity;

      adjacency_list_[--first][--second] += capacity;
      residual_list_[first][second] += capacity;
    }
  }
};

int main() {
  Graph graph;

  graph.InputTheGraph();

  std::cout << graph.FordFulkerson() << std::endl;

  return 0;
}
