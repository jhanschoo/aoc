#include <bits/stdc++.h>
#include <range/v3/all.hpp>

namespace {
    using ll = long long;
    using ull = unsigned long long;
}

template <typename T>
class AVLTree {
public:
private:
    struct Node {
        std::shared_ptr<Node> left, right;
        int balance;
        std::size_t ind;
        T value;
    };
    std::shared_ptr<Node> root;

    void insert(T value, std::size_t ind) {
        if (!root) {
            root = std::make_shared<Node>(Node{nullptr, nullptr, 0, 0, value});
            return;
        }
        auto current = root;
        auto ancestors = std::vector<std::shared_ptr<Node>>{};
        while (true) {
            // If our index is not greater than the current node's index, we insert in the left subtree;
            // if the left subtree is empty, then we have current->ind == ind.
            ancestors.push_back(current);
            if (ind <= current->ind) {
                ++(current->ind);
                if (!current->left) {
                    current->left = std::make_shared<Node>(Node{nullptr, nullptr, 0, ind, value});
                    ancestors.push_back(current->left);
                    break;
                }
                current = current->left;
            } else {
                if (!current->right) {
                    current->right = std::make_shared<Node>(Node{nullptr, nullptr, 0, current->ind + 1, value});
                    ancestors.push_back(current->right);
                    break;
                }
            }
        }
    }
};

int main() {
    auto numbers = std::vector<int>{};
    ranges::copy(std::istream_iterator<int>(std::cin), std::istream_iterator<int>(), ranges::back_inserter(numbers));
    auto num_numbers = static_cast<int>(numbers.size());
    std::cout << numbers.size() << std::endl;
    for (auto n: numbers) {
        std::cout << n << ' ' << n % num_numbers << std::endl;
    }
}
