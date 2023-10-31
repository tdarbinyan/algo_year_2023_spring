#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>

class Bor {
 private:
  struct Vertex {
    std::map<char, Vertex*> next;
    std::map<char, Vertex*> to;
    Vertex* prev = nullptr;
    Vertex* link = nullptr;
    Vertex* comp = nullptr;
    bool term = false;
    char to_curr;
    std::vector<size_t> order;

    Vertex() { link = comp = this; }

    Vertex(Vertex* prev, char to_curr) : prev(prev), to_curr(to_curr) {}

    Vertex* Link(Vertex* root) {
      if (link == nullptr) {
        if (this == root || prev == root) {
          link = root;
        } else {
          link = prev->Link(root)->To(root, to_curr);
        }
      }
      return link;
    }

    Vertex* To(Vertex* root, char ch) {
      if (!to.count(ch)) {
        if (next.count(ch)) {
          to[ch] = next[ch];
        } else if (this != root) {
          if (link == nullptr) {
            to[ch] = this->Link(root)->To(root, ch);
          } else {
            to[ch] = this->link->To(root, ch);
          }
        } else {
          to[ch] = root;
        }
      }
      return to[ch];
    }

    Vertex* Comp(Vertex* root) {
      if (comp == nullptr) {
        if (this->Link(root)->term || this->Link(root) == root) {
          comp = this->Link(root);
        } else {
          comp = this->Link(root)->Comp(root);
        }
      }
      return comp;
    }
  };

  Vertex* root_;
  std::vector<std::string> patterns_;
  std::vector<size_t> psizes_;
  std::vector<std::vector<size_t>> ans_;

 public:
  void InsertPatterns(size_t& num) {
    patterns_.resize(num);
    psizes_.resize(num);

    for (size_t i = 0; i < num; ++i) {
      std::cin >> patterns_[i];
      psizes_[i] = patterns_[i].size();
    }
  }

  void AddPattern(std::string& pattern, size_t index) {
    Vertex* current = root_;
    for (auto ch : pattern) {
      if (current->next.count(ch)) {
        current = current->next[ch];
      } else {
        current = current->next[ch] = new Vertex(current, ch);
      }
    }
    current->order.push_back(index);
    current->term = true;
  }

  void AddPatterns() {
    for (size_t i = 0; i < patterns_.size(); ++i) {
      AddPattern(patterns_[i], i);
    }
  }

  void AddLinksBFS() {
    std::queue<Vertex*> queue;
    Vertex* current = root_;
    queue.push(root_);

    while (!current->next.empty()) {
      current = queue.front();

      for (auto& temp : current->next) {
        if (temp.second->link == nullptr) {
          temp.second->Link(root_);
          queue.push(temp.second);
        }
      }

      queue.pop();
    }
  }

  void ConstructBor() {
    root_ = new Vertex();

    AddPatterns();

    AddLinksBFS();
  }

  void Corasick(std::string text) {
    ans_.resize(patterns_.size());
    Vertex* current = root_;

    size_t iter = 1;

    for (auto& ch : text) {
      current = current->To(root_, ch);
      if (current->term) {
        for (auto& i : current->order) {
          ans_[i].push_back(iter);
        }
      }
      Vertex* temp = current;
      while (temp->Comp(root_) != root_) {
        temp = temp->comp;
        for (auto& i : temp->order) {
          ans_[i].push_back(iter);
        }
      }

      ++iter;
    }
  }

  void OutputAnswer() {
    for (size_t i = 0; i < ans_.size(); ++i) {
      std::cout << ans_[i].size();
      for (auto& iter : ans_[i]) {
        std::cout << ' ' << iter - psizes_[i] + 1;
      }
      std::cout << '\n';
    }
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  std::string text;
  size_t num;
  Bor bor;

  std::cin >> text >> num;

  bor.InsertPatterns(num);

  bor.ConstructBor();

  bor.Corasick(text);

  bor.OutputAnswer();
}
