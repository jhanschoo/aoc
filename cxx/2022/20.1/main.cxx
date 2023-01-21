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
        auto ancestors = std::vector<std::shared_ptr<Node>>{root};
        {
            auto current = root;
            while (true) {
                // we know that we are going to insert into a child of current
                ++(current->sz);
                auto current_idx = parent_left_idx + (current->left ? current->left->sz : 0);
                // If our index is lte to the current node's index, we insert in the left subtree; hence
                if (idx <= current_idx) {
                    // note no update of parent_left_idx
                    // case where left subtree is empty, then we make a leaf node.
                    if (!current->left) {
                        current->left = leaf_node;
                        ancestors.push_back(current->left);
                        break;
                    }
                    current = current->left;
                    ancestors.push_back(current->left);
                } else {
                    // Suppose that current's idx is 5, and current->right->left->sz is 3. Then, current->right's idx is 5 + 1 + 3 = 9.
                    // Intuition to the + 1 is that we want to convert from 0-based indexing of the last element to sizes.
                    parent_left_idx = current_idx + 1;
                    // case where right subtree is empty, then we make a leaf node.
                    if (!current->right) {
                        current->right = leaf_node;
                        ancestors.push_back(current->right);
                        break;
                    }
                    current = current->right;
                    ancestors.push_back(current->right);
                }
            }
        }
        // ancestors contains the path from the root to the newly inserted node.
        // Note that `ancestors.size()` is at least 2, since we always insert into a descendent of the root.
        for (auto parent = ancestors.back(); i > 0; --i) {

            auto &current = ancestors[i];
            auto &parent = ancestors[i - 1];
            if (parent->left == current) {
                --(parent->balance);
            } else {
                ++(parent->balance);
            }
            if (parent->balance == 0) {
                break;
            }
            if (parent->balance == -2) {
                if (current->balance == 1) {
                    // left-right case
                    rotate_left(current);
                }
                // left-left case
                rotate_right(parent);
                break;
            }
            if (parent->balance == 2) {
                if (current->balance == -1) {
                    // right-left case
                    rotate_right(current);
                }
                // right-right case
                rotate_left(parent);
                break;
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
