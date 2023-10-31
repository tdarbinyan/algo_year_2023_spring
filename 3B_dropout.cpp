#include <iostream>
#include <vector>
#include <string>

class Heuristics {
private:
    size_t size = 0;
    std::vector<size_t> parent;
    std::vector<size_t> power;

public:
    Heuristics(size_t& size) {
        this->parent.resize(size);
        this->power.resize(size);
        for (size_t i = 0; i < size; i++) {
            parent[i] = i;
            power[i] = 0;
        }
    }

    size_t Find(size_t& vert) {
        size_t root = vert;
        while (root != this->parent[root]) {
            root = parent[root];
        }

        while (vert != root) {
            size_t temp = this->parent[vert];
            this->parent[vert] = root;
            vert = temp;
        }
        return root;
    }

    void Cut(size_t& first, size_t& second) {

        first = Find(first);
        second = Find(second);

        if (first == second) {
            ++this->power[first];
            return;
        }

        if (this->power[first] < this->power[second]) {
            std::swap(first, second);
        }

        this->parent[second] = first;
        this->power[first] += this->power[second] + 1;

        if (this->power[first] == this->power[second]) {
            ++this->power[first];
        }
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    size_t v_quant = 0;
    size_t e_quant = 0;
    size_t quer = 0;

    std::vector<std::string> ans;
    std::vector<std::pair<std::string, std::pair<size_t, size_t>>> requests;

    std::cin >> v_quant >> e_quant >> quer;
    Heuristics g(v_quant);

    for (size_t i = 0; i < e_quant; ++i) {
        size_t lol, kek;
        std::cin >> lol >> kek;
    }

    for (size_t i = 0; i < quer; ++i) {
        std::string cheburek;
        size_t lol, kek;
        std::cin >> cheburek >> lol >> kek;
        requests.push_back(std::make_pair(cheburek, std::make_pair(--lol, --kek)));
    }
    for (size_t i = quer; i > 0; --i) {
        if (requests[i - 1].first == "ask") {
            if (g.Find(requests[i - 1].second.first) ==
                g.Find(requests[i - 1].second.second)) {
                ans.push_back("YES\n");
                continue;
            }
            ans.push_back("NO\n");
        }
        else {
            g.Cut(requests[i - 1].second.first, requests[i - 1].second.second);
        }
    }
    for (size_t i = ans.size(); i > 0; --i) {
        std::cout << ans[i - 1];
    }
}
