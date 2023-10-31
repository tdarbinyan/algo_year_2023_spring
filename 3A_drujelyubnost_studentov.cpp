#include <iostream>
#include <string>
#include <vector>

template <typename T = int32_t>
class Heuristics {
private:
    size_t size_ = 0;
    std::vector<size_t> parent_;
    std::vector<T> power_;

public:
    Heuristics(size_t& size) {
        this->parent_.resize(size);
        this->power_.resize(size, 0);
        for (size_t i = 0; i < size; i++) {
            this->parent_[i] = i;
        }
    }

    size_t Find(size_t& vertex) {
        size_t root = vertex;
        while (root != this->parent_[root]) {
            root = this->parent_[root];
        }
        return root;
    }

    void Unite(size_t& first, size_t& second, T& power) {
        first = Find(first);
        second = Find(second);

        if (first == second) {
            this->power_[first] += power;
            return;
        }

        if (this->power_[first] < this->power_[second]) {
            std::swap(first, second);
        }

        this->parent_[second] = first;
        this->power_[first] += this->power_[second] + power;

        if (this->power_[first] == this->power_[second]) {
            this->power_[first] += power;
        }
    }

    T& GetPower(size_t index) { return this->power_[index]; }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    size_t size;
    size_t query;

    std::cin >> size >> query;
    Heuristics<size_t> heur(size);

    for (size_t i = 0; i < query; ++i) {
        int command;
        std::cin >> command;
        if (command == 1) {
            size_t first;
            size_t second;
            size_t power;
            std::cin >> first >> second >> power;
            heur.Unite(--first, --second, power);
        }
        else {
            size_t vert;
            std::cin >> vert;
            std::cout << heur.GetPower(heur.Find(--vert)) << "\n";
        }
    }
}
