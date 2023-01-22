#include <bits/stdc++.h>
#include <range/v3/all.hpp>

struct elem_t {
    std::string left, right;
    char op;
    long long value;
};

namespace {
    using ll = long long;
    using ull = unsigned long long;
}

int main() {
    auto m = std::map<std::string, elem_t>{};
    {
        auto label = std::string{}, left = std::string{}, right = std::string{};
        auto op = char{};
        auto value = ll{};
        while (std::cin >> label >> std::ws) {
            label.resize(4);
            if (isdigit(std::cin.peek())) {
                std::cin >> value;
                m[label] = {"", "", '\0', value};
            } else {
                std::cin >> left >> op >> right;
                m[label] = {left, right, op, 0};
            }
        }
    }
    auto unvisited = m | ranges::views::keys | ranges::to<std::set<std::string>>;
    auto sorted = std::vector<std::string>{};
    while (!unvisited.empty()) {
        auto stack = std::vector<std::pair<std::string, bool>>{std::pair{*unvisited.begin(), false}};
        while (!stack.empty()) {
            auto &[label, visited] = stack.back();
            if (visited) {
                stack.pop_back();
                unvisited.erase(label);
                sorted.push_back(label);
                continue;
            }
            if (!unvisited.contains(label)) {
                stack.pop_back();
                continue;
            }
            auto &[left, right, op, value] = m[label];
            visited = true;
            if (op == '\0') {
                continue;
            }
            stack.emplace_back(right, false);
            stack.emplace_back(left, false);
        }
    }
    for (const auto& label : sorted) {
        auto &[left, right, op, value] = m[label];
        switch (op) {
            case '+':
                value = m[left].value + m[right].value;
                break;
            case '-':
                value = m[left].value - m[right].value;
                break;
            case '*':
                value = m[left].value * m[right].value;
                break;
            case '/':
                value = m[left].value / m[right].value;
                break;
            default:
                break;
        }
    }
    std::cout << m["root"].value << std::endl;
}
