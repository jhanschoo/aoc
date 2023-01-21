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
        std::size_t sz;
        T value;
    };
    std::shared_ptr<Node> root;

    void insert(T value, std::size_t idx) {
        auto leaf_node = std::make_shared<Node>(nullptr, nullptr, 0, 1, value);
        if (!root) {
            root = leaf_node;
            return;
        }
        // parent_left_idx accumulates the number of nodes that contribute to the current node's index that
        // are not in the left subtree of the current node
        auto parent_left_idx = std::size_t{0};
        auto current = root;
        auto ancestors = std::vector<std::shared_ptr<Node>>{current};
        while (true) {
            // we know that we are going to insert into a child of current
            ++(current->sz);
            auto current_idx = parent_left_idx + (current->left ? current->left->sz : 0);
            // If our index is lte to the current node's index, we insert in the left subtree; hence
            if (idx <= current_idx) {
                // note no update of parent_left_idx
                // if the left subtree is empty, then we make a leaf node.
                if (!current->left) {
                    current->left = leaf_node;
                    ancestors.push_back(current->left);
                    break;
                }
                current = current->left;
            } else {
                // update current node's balance
                current->balance += 1;
                parent_left_idx = current_idx;
                if (!current->right) {
                    current->right = leaf_node;
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
