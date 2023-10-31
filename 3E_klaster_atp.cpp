#include <algorithm>
#include <iostream>
#include <vector>
#include <limits>

class Heuristics {
private:
    size_t size_ = 0;
    size_t weight_ = 0;
    std::vector<bool> visited_;
    std::vector<size_t> parent_;
    std::vector<size_t> power_;
    std::vector<std::pair<size_t, std::pair<size_t, size_t>>> edges_;
    std::vector<std::vector<std::pair<size_t, size_t>>> min_span_tree_;

public:
    Heuristics(size_t& size) {
        this->min_span_tree_.resize(size);
        this->parent_.resize(size);
        this->power_.resize(size, 0);
        for (size_t i = 0; i < size; i++) {
            this->parent_[i] = i;
        }
    }

    size_t Find(size_t& vertex) {
        if (vertex == this->parent_[vertex]) {
            return vertex;
        }
        this->parent_[vertex] = Find(this->parent_[vertex]);
        return this->parent_[vertex];
    }

    void Unite(size_t& first, size_t& second) {
        first = Find(first);
        second = Find(second);
        if (first != second) {
            this->parent_[first] = second;
        }
    }

    void AddMstEdge(std::pair<size_t, size_t>& edge, size_t& length) {
        min_span_tree_[edge.second].push_back(std::make_pair(length, edge.first));
        min_span_tree_[edge.first].push_back(std::make_pair(length, edge.second));
    }

    size_t FindWay(size_t start, size_t finish, size_t cost) {
        if (start == finish) {
            return cost;
        }
        visited_[start] = true;
        size_t result = 0;
        for (const auto& vertex : min_span_tree_[start]) {
            if (visited_[vertex.second]) { continue; }
            result = FindWay(vertex.second, finish, (cost > vertex.first ? vertex.first : cost));
            if (result > 0) {
                return result > cost ? cost : result;
            }
        }
        return result;
    }

    size_t KruskalReturnWeight(size_t query) {
        std::sort(this->edges_.rbegin(), this->edges_.rend());
        for (int i = 0; i < query; ++i) {
            if (Find(this->edges_[i].second.first) != Find(this->edges_[i].second.second)) {
                this->weight_ += this->edges_[i].first;
                AddMstEdge(this->edges_[i].second, this->edges_[i].first);
                Unite(this->edges_[i].second.first, this->edges_[i].second.second);
            }
        }
        return weight_;
    }

    std::vector<std::pair<size_t, std::pair<size_t, size_t>>>& GetEdges() {
        return edges_;
    }

    std::size_t GetWeight() {
        return weight_;
    }

    std::vector<bool>& GetVisited() {
        return visited_;
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    size_t vertices, edges, query;
    std::cin >> vertices >> edges >> query;

    Heuristics heur(vertices);
    for (size_t i = 0; i < edges; ++i) {
        size_t first, second, length;
        std::cin >> first >> second >> length;
        heur.GetEdges().push_back(std::make_pair(length, std::make_pair(--first, --second)));
    }

    heur.KruskalReturnWeight(edges);

    for (size_t i = 0; i < query; ++i) {
        size_t start, finish;
        std::cin >> start >> finish;
        heur.GetVisited().assign(vertices, false);
        std::cout << heur.FindWay(--start, --finish, std::numeric_limits<size_t>::max()) << "\n";
    }

    return 0;
}
