#include <iostream>
#include <vector>
#include <algorithm>

class MSTree {
    size_t total_weight = 0;
    std::vector<size_t> power;
    std::vector<size_t> parent;
    std::vector<std::pair<size_t, std::pair<size_t, size_t>>> edges;
public:
    MSTree(size_t& size) {
        this->parent.resize(size);
        this->power.resize(size, 0);
        for (size_t i = 0; i < size; ++i) {
            this->parent[i] = i;
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

    size_t KruskalReturnWeight(size_t& query) {
        std::sort(this->edges.begin(), this->edges.end());

        for (size_t i = 0; i < query; ++i) {
            if (Find(this->edges[i].second.first) !=
                Find(this->edges[i].second.second)) {
                this->total_weight += this->edges[i].first;
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
    std::cout.tie(nullptr);
    size_t size, query;
    std::cin >> size >> query;
    MSTree heur(size);
    size_t first, second, weight;
    for (size_t i = 0; i < query; ++i) {
        std::cin >> first >> second >> weight;
        heur.GetEdges().push_back(std::make_pair(weight, std::make_pair(first - 1, second - 1)));
    }
    std::cout << heur.KruskalReturnWeight(query) << "\n";
}
