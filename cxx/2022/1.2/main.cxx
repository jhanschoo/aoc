#include <bits/stdc++.h>
#include <range/v3/all.hpp>

int main() {
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);

    auto maximums = std::array{0, 0, 0};
    auto current = int{0};
    auto s = std::string{};
    auto update = [&]() {
        if (current) {
            auto smallest = ranges::min_element(maximums);
            *smallest = std::max(*smallest, current);
            current = 0;
        }
    };
    while (getline(std::cin, s)) {
        if (s.empty()) {
            update();
        } else {
            auto n{std::stoi(s)};
            current += n;
        }
    }
    update();
    auto sum = ranges::accumulate(maximums, 0);
    std::cout << sum << std::endl;
}
