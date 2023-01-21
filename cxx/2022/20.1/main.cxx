#include <bits/stdc++.h>
#include <range/v3/all.hpp>
#include <boost/assert.hpp>

namespace {
    using ll = long long;
    using ull = unsigned long long;
}

//template <typename T>
class AVLTree {
public:
    void insert(std::size_t idx, const std::string &value) {
        auto leaf_node = std::make_unique<Node>(Node{nullptr, nullptr, 0, 1, value});
        if (!root) {
            root = std::move(leaf_node);
            return;
        }
        auto ancestors = std::vector<std::reference_wrapper<std::unique_ptr<Node>>>{root};
        // parent_left_idx accumulates the number of nodes that contribute to the current node's index that
        // are not in the left subtree of the current node
        auto parent_left_idx = std::size_t{0};
        while (leaf_node) {
            auto &current = ancestors.back().get();
            // we know that we are going to insert into a child of current
            ++(current->sz);
            auto current_idx = parent_left_idx + get_sz(current->left);
            // If our index is lte to the current node's index, we insert in the left subtree; hence
            if (idx <= current_idx) {
                // note no update of parent_left_idx
                // case where left subtree is empty, then we make a leaf node.
                if (!current->left) {
                    current->left = std::move(leaf_node);
                }
                ancestors.emplace_back(current->left);
            } else {
                // Suppose that current's idx is 5, and current->right->left->sz is 3. Then, current->right's idx is 5 + 1 + 3 = 9.
                // Intuition to the + 1 is that we want to convert from 0-based indexing of the last element to sizes.
                parent_left_idx = current_idx + 1;
                // case where right subtree is empty, then we make a leaf node.
                if (!current->right) {
                    current->right = std::move(leaf_node);
                }
                ancestors.emplace_back(current->right);
            }
        }
        // We now update the balance factors of the ancestors of the inserted node.
        auto current_wrap = ancestors.back();
        ancestors.pop_back();
        // Invariant: the subtree rooted at current_wrap.get() is balanced, and its height has increased by 1.
        for (auto &&ancestor_wrap: ancestors | ranges::views::reverse) {
            auto &parent = ancestor_wrap.get(), &current = current_wrap.get();
            auto is_known_balanced = false;
            if (&(parent->right) == &current) {
                switch (parent->balance) {
                    case 1:
                        switch (current->balance) {
                            case -1:
                                bookkept_rotate_right_left(parent);
                                break;
                            default:
                                bookkept_rotate_left(parent);
                                break;
                        }
                        is_known_balanced = true;
                        break;
                    case 0:
                        parent->balance = 1;
                        current_wrap = ancestor_wrap;
                        break;
                    case -1:
                        parent->balance = 0;
                        is_known_balanced = true;
                        break;
                }
            } else {
                switch (parent->balance) {
                    case -1:
                        switch (current->balance) {
                            case 1:
                                bookkept_rotate_left_right(parent);
                                break;
                            default:
                                bookkept_rotate_right(parent);
                                break;
                        }
                        is_known_balanced = true;
                        current_wrap = ancestor_wrap;
                        break;
                    case 0:
                        parent->balance = -1;
                        current_wrap = ancestor_wrap;
                        break;
                    case 1:
                        parent->balance = 0;
                        is_known_balanced = true;
                        break;
                }
            }
            if (is_known_balanced) {
                break;
            }
        }
    }

    void erase(std::size_t idx) {
        if (!root) {
            throw std::runtime_error("Invalid idx");
        }
        auto ancestors = std::vector<std::reference_wrapper<std::unique_ptr<Node>>>{root};
        {
            // parent_left_idx accumulates the number of nodes that contribute to the current node's index that
            // are not in the left subtree of the current node
            auto parent_left_idx = std::size_t{0};
            auto target_wrap = std::optional<std::reference_wrapper<std::unique_ptr<Node>>>{};
            // first find the target node
            while (!target_wrap) {
                auto &current = ancestors.back().get();
                --(current->sz);
                auto current_idx = parent_left_idx + get_sz(current->left);
                if (idx == current_idx) {
                    target_wrap = ancestors.back();
                } else if (idx < current_idx) {
                    BOOST_ASSERT_MSG(current->left, "Invalid idx");
                    ancestors.emplace_back(current->left);
                } else {
                    // Suppose that current's idx is 5, and current->right->left->sz is 3. Then, current->right's idx is 5 + 1 + 3 = 9.
                    // Intuition to the + 1 is that we want to convert from 0-based indexing of the last element to sizes.
                    parent_left_idx = current_idx + 1;
                    if (!current->right) {
                        throw std::runtime_error("Invalid idx");
                    }
                    ancestors.emplace_back(current->right);
                }
            }
            // now we have the target node
            // we perform the deletion of the sacrificed node
            // Invariant: target_wrap == ancestors.back()
            auto &target = target_wrap->get();
            // Case where target->left is empty
            if (!target->left) {
                // we delete the target node
                target = std::move(target->right);
            } else {
                // Case where target->left is not empty
                // We find the predecessor of target
                ancestors.emplace_back(target->left);
                while (ancestors.back().get()->right) {
                    auto &current = ancestors.back().get();
                    --(current->sz);
                    ancestors.emplace_back(current->right);
                }
                // now we have the predecessor
                auto &predecessor = ancestors.back().get();
                auto &predecessor_ref = *predecessor, &target_ref = *target;
                debug_print();
                std::cout << "1" << "predecessor: " << predecessor_ref.value << ", target: " << target_ref.value << std::endl;
                auto target_left = std::move(target_ref.left);
                std::cout << "2" << "predecessor: " << predecessor_ref.value << ", target: " << target_ref.value << std::endl;
                auto target_right = std::move(target_ref.right);
                std::cout << "3" << "predecessor: " << predecessor_ref.value << ", target: " << target_ref.value << std::endl;
                auto predecessor_left = std::move(predecessor_ref.left);
                std::cout << "4" << "predecessor: " << predecessor_ref.value << ", target: " << target_ref.value << std::endl;
                auto predecessor_right = std::move(predecessor_ref.right);
                std::cout << "5" << "predecessor: " << predecessor_ref.value << ", target: " << target_ref.value << std::endl;
                std::swap(target_ref.balance, predecessor_ref.balance);
                std::cout << "6" << "predecessor: " << predecessor_ref.value << ", target: " << target_ref.value << std::endl;
                std::swap(target_ref.sz, predecessor_ref.sz);
                std::cout << "7" << "predecessor: " << predecessor_ref.value << ", target: " << target_ref.value << std::endl;

                // target will be in predecessor's subtree so we splice target back first to avoid
                // losing the unique_ptr to it
                target_ref.left = std::move(predecessor_left);
                std::cout << "8" << "predecessor: " << predecessor_ref.value << ", target: " << target_ref.value << std::endl;
                target_ref.right = std::move(predecessor_right);
                std::cout << "9" << "predecessor: " << predecessor_ref.value << ", target: " << target_ref.value << std::endl;
                predecessor_ref.left = std::move(target_left);
                std::cout << "10" << "predecessor: " << predecessor_ref.value << ", target: " << target_ref.value << std::endl;
                predecessor_ref.right = std::move(target_right);
                std::cout << "11" << "predecessor: " << predecessor_ref.value << ", target: " << target_ref.value << std::endl;

                // we delete the target, now spliced into a leaf node
//                target = std::move(target->left);
                std::cout << "12" << "predecessor: " << predecessor_ref.value << ", target: " << target_ref.value << std::endl;
                debug_print();
                ;
            }
        }
        // We now perform the rotations to restore the balance of the tree.
        // Invariant: ancestors.back() points to a (possibly empty) tree whose height is 1 less than the original tree.
        // and this tree satisfies the AVL property. However, the ancestors of this tree
        // may not satisfy the AVL property.
        auto current_wrap = ancestors.back();
        ancestors.pop_back();
        for (auto &&ancestor_wrap: ancestors | ranges::views::reverse) {
            auto &parent = ancestor_wrap.get(), &current = current_wrap.get();
            auto is_known_balanced = false;
            if (&(parent->right) == &current) {
                switch (parent->balance) {
                    case 1:
                        parent->balance = 0;
                        current_wrap = ancestor_wrap;
                        break;
                    case 0:
                        parent->balance = -1;
                        is_known_balanced = true;
                        break;
                    case -1:
                        auto balance = parent->left->balance;
                        if (balance == 1) {
                            bookkept_rotate_left_right(parent);
                        } else {
                            bookkept_rotate_right(parent);
                        }
                        if (balance == 0) {
                            is_known_balanced = true;
                        } else {
                            current_wrap = ancestor_wrap;
                        }
                        break;
                }
            } else {
                switch (parent->balance) {
                    case -1:
                        parent->balance = 0;
                        current_wrap = ancestor_wrap;
                        break;
                    case 0:
                        parent->balance = 1;
                        is_known_balanced = true;
                        break;
                    case 1:
                        auto balance = parent->right->balance;
                        if (balance == -1) {
                            bookkept_rotate_right_left(parent);
                        } else {
                            bookkept_rotate_left(parent);
                        }
                        if (balance == 0) {
                            is_known_balanced = true;
                        } else {
                            current_wrap = ancestor_wrap;
                        }
                        break;
                }
            }
            if (is_known_balanced) {
                break;
            }
        }
    }

    [[nodiscard]] std::size_t size() const {
        return get_sz(root);
    }

    [[nodiscard]] bool empty() const {
        return !root;
    }

    void debug_print() const {
        debug_print_subtree(root);
        std::cout << std::endl;
    }

    ~AVLTree() {
        if (!root) {
            return;
        }
        auto stack = std::vector<std::unique_ptr<Node>>{};
        stack.push_back(std::move(root));
        while (!stack.empty()) {
            auto node = std::move(stack.back());
            stack.pop_back();
            if (node->left) {
                stack.push_back(std::move(node->left));
            }
            if (node->right) {
                stack.push_back(std::move(node->right));
            }
        }
    }
private:
    struct Node {
        std::unique_ptr<Node> left, right;
        signed char balance;
        std::size_t sz;
        std::string value;
    };
    std::unique_ptr<Node> root;

    static std::size_t get_sz(const std::unique_ptr<Node> &node) {
        return node ? node->sz : 0;
    }

    static void update_sz(std::unique_ptr<Node> &node) {
        node->sz = get_sz(node->left) + get_sz(node->right) + 1;
    }

    static void rotate_left(std::unique_ptr<Node> &parent) {
        // perform rotation
        auto x = std::move(parent);
        parent = std::move(x->right);
        x->right = std::move(parent->left);
        parent->left = std::move(x);

        // update sizes
        parent->left->sz = get_sz(parent->left->left) + get_sz(parent->left->right) + 1;
        parent->sz = get_sz(parent->left) + get_sz(parent->right) + 1;
    }

    static void rotate_right(std::unique_ptr<Node> &parent) {
        // perform rotation
        auto x = std::move(parent);
        parent = std::move(x->left);
        x->left = std::move(parent->right);
        parent->right = std::move(x);

        // update sizes
        parent->right->sz = get_sz(parent->right->left) + get_sz(parent->right->right) + 1;
        parent->sz = get_sz(parent->left) + get_sz(parent->right) + 1;
    }

    static void rotate_right_left(std::unique_ptr<Node> &parent) {
        rotate_right(parent->right);
        rotate_left(parent);
    }

    static void rotate_left_right(std::unique_ptr<Node> &parent) {
        rotate_left(parent->left);
        rotate_right(parent);
    }

    static void bookkept_rotate_left(std::unique_ptr<Node> &parent) {
        rotate_left(parent);
        // update sizes
        update_sz(parent->left);
        update_sz(parent);
        // update balance factors
        switch (parent->balance) {
            case 0:
                parent->left->balance = 1;
                parent->balance = -1;
                break;
            case 1:
                parent->left->balance = 0;
                parent->balance = 0;
                break;
            default:
                BOOST_ASSERT_MSG(false, "Invalid balance factor");
        }
    }

    static void bookkept_rotate_right(std::unique_ptr<Node> &parent) {
        rotate_right(parent);
        // update sizes
        update_sz(parent->right);
        update_sz(parent);
        // update balance factors
        switch (parent->balance) {
            case 0:
                parent->right->balance = -1;
                parent->balance = 1;
                break;
            case -1:
                parent->right->balance = 0;
                parent->balance = 0;
                break;
            default:
                BOOST_ASSERT_MSG(false, "Invalid balance factor");
        }
    }

    static void bookkept_rotate_right_left(std::unique_ptr<Node> &parent) {
        rotate_right_left(parent);
        // update sizes
        update_sz(parent->right);
        update_sz(parent->left);
        update_sz(parent);
        // update balance factors
        switch (parent->balance) {
            case -1:
                parent->left->balance = 0;
                parent->right->balance = 1;
                break;
            case 0:
                parent->left->balance = 0;
                parent->right->balance = 0;
                break;
            case 1:
                parent->left->balance = -1;
                parent->right->balance = 0;
                break;
            default:
                BOOST_ASSERT_MSG(false, "Invalid balance factor");
        }
        parent->balance = 0;
    }

    static void bookkept_rotate_left_right(std::unique_ptr<Node> &parent) {
        rotate_left_right(parent);
        // update sizes
        update_sz(parent->right);
        update_sz(parent->left);
        update_sz(parent);
        // update balance factors
        switch (parent->balance) {
            case -1:
                parent->left->balance = 0;
                parent->right->balance = 1;
                break;
            case 0:
                parent->left->balance = 0;
                parent->right->balance = 0;
                break;
            case 1:
                parent->left->balance = -1;
                parent->right->balance = 0;
                break;
            default:
                BOOST_ASSERT_MSG(false, "Invalid balance factor");
        }
        parent->balance = 0;
    }

    static void debug_print_subtree(const std::unique_ptr<Node> &node, std::size_t depth = 0) {
        for ([[maybe_unused]] auto i_ : ranges::views::iota(std::size_t{0}, depth)) {
            std::cout << "  ";
        }
        if (!node) {
            std::cout << "[]";
            return;
        }
        std::cout << "[" << node->value << ", " << static_cast<int>(node->balance) << ", " << node->sz << "," << std::endl;
        debug_print_subtree(node->left, depth + 2);
        std::cout << ", " << std::endl;
        debug_print_subtree(node->right, depth + 2);
        std::cout << ", " << std::endl;
        for ([[maybe_unused]] auto i_ : ranges::views::iota(std::size_t{0}, depth)) {
            std::cout << "  ";
        }
        std::cout << "]";
    }
};

int main() {
    auto t = AVLTree{};
    for (auto [val, idx] : std::array{
            std::pair{std::string{"M"}, 0},
            std::pair{std::string{"N"}, 1},
            std::pair{std::string{"O"}, 2},
            std::pair{std::string{"L"}, 0},
            // error here
            std::pair{std::string{"K"}, 0},
            std::pair{std::string{"Q"}, 5},
            std::pair{std::string{"P"}, 5},
            std::pair{std::string{"H"}, 0},
            std::pair{std::string{"I"}, 1},
            std::pair{std::string{"A"}, 0},
    }) {
        t.insert(idx, val);
        t.debug_print();
    }

    t.erase(t.size() / 2);
    while (!t.empty()) {
        t.erase(t.size() / 2);
        t.debug_print();
    }

//    auto numbers = std::vector<int>{};
//    ranges::copy(std::istream_iterator<int>(std::cin), std::istream_iterator<int>(), ranges::back_inserter(numbers));
//    auto num_numbers = static_cast<int>(numbers.size());
//    std::cout << numbers.size() << std::endl;
//    for (auto n: numbers) {
//        std::cout << n << ' ' << n % num_numbers << std::endl;
//    }
//    auto a = std::make_shared<int>(25);
//    auto va = std::vector<std::reference_wrapper<std::shared_ptr<int>>>{a};
//    auto aref = va.back().get();
//    *aref = 26;
//    std::cout << *a << std::endl;
}
