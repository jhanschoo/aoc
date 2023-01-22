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
    // we mark elements with finalized values by setting their op to '\0'
    for (const auto& label : sorted) {
        auto &[left, right, op, value] = m[label];
        if (label == "humn") {
            op = ' ';
            continue;
        }
        auto &m_left = m[left], m_right = m[right];
        if (m_left.op == '\0' && m_right.op == '\0') {
            switch (op) {
                case '+':
                    value = m_left.value + m_right.value;
                    break;
                case '-':
                    value = m_left.value - m_right.value;
                    break;
                case '*':
                    value = m_left.value * m_right.value;
                    break;
                case '/':
                    value = m_left.value / m_right.value;
                    break;
                default:
                    break;
            }
            op = '\0';
        }
    }
    {
        // process root
        auto &[left, right, op, value] = m["root"];
        value = (m[left].value + m[right].value) * 2;
        op = '+';
    }
    for (const auto& label : sorted | ranges::views::reverse) {
        auto &[left, right, op, value] = m[label];
        if (op == '\0') {
            continue;
        }
        auto &m_left = m[left], &m_right = m[right];
        if (m_left.op != '\0') {
            switch (op) {
                case '+':
                    // left + right = value => left = value - right
                    m_left.value = value - m_right.value;
                    break;
                case '-':
                    // left - right = value => left = value + right
                    m_left.value = value + m_right.value;
                    break;
                case '*':
                    // left * right = value => left = value / right
                    m_left.value = value / m_right.value;
                    break;
                case '/':
                    // left / right = value => left = value * right
                    m_left.value = value * m_right.value;
                    break;
                default:
                    break;
            }
        } else if (m_right.op != '\0') {
            switch (op) {
                case '+':
                    // left + right = value => right = value - left
                    m_right.value = value - m_left.value;
                    break;
                case '-':
                    // left - right = value => right = left - value
                    m_right.value = m_left.value - value;
                    break;
                case '*':
                    // left * right = value => right = value / left
                    m_right.value = value / m_left.value;
                    break;
                case '/':
                    // left / right = value => right = left / value
                    m_right.value = m_left.value / value;
                    break;
                default:
                    break;
            }
        }
    }
    std::cout << m["humn"].value << std::endl;
}
