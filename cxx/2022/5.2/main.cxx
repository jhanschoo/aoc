#include <bits/stdc++.h>
#include <range/v3/all.hpp>

int main() {
    std::string line;
    auto schematic = std::vector<std::string>{};
    while (std::getline(std::cin, line)) {
        // if we have reached the stack index numbers
        if (line.size() >= 2 && line[1] == '1') {
            break;
        } else {
            schematic.push_back(line);
        }
    }
    auto stacks = std::vector<std::vector<char>>((line.size() + 2) / 4);
    for (const auto &row: schematic | ranges::view::reverse) {
        auto crates = row | ranges::view::drop(1) | ranges::view::stride(4);
        ranges::accumulate(crates, stacks.begin(), [](auto &sit, const auto &crate) {
            if (crate != ' ') {
                sit->push_back(crate);
            }
            return ++sit;
        });
    }
    std::string s1, s2, s3;
    std::size_t from, to;
    long count;
    while (std::cin >> s1 >> count >> s2 >> from >> s3 >> to) {
        --from;
        --to;
        std::move(stacks[from].end() - count, stacks[from].end(), std::back_inserter(stacks[to]));
        stacks[from].resize(stacks[from].size() - count);
    }
    for (auto &&stack: stacks) {
        std::cout << (stack.empty() ? ' ' : stack.back());
    }
    std::cout << std::endl;
}
