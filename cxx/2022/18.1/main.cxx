#include <bits/stdc++.h>
#include <range/v3/all.hpp>

namespace {
    using ll = long long;
    using ull = unsigned long long;
    using sz = std::size_t;
}

template<typename Func, int ind = 0>
void neighbors_for_each(const Func &f) {
    if constexpr (ind < 27) {
        const auto x = ind % 3 - 1;
        const auto y = ind / 3 % 3 - 1;
        const auto z = ind / 9 % 3 - 1;
        if (std::abs(x) + std::abs(y) + std::abs(z) == 1) {
            f(x, y, z);
        }
        neighbors_for_each<Func, ind + 1>(f);
    }
}

int main() {
    auto exposed = 0ll;
    auto drops = std::set<std::array<int, 3>>{};
    auto buffer = char{};
    auto x = 0, y = 0, z = 0;
    while (std::cin >> x >> buffer >> y >> buffer >> z) {
        drops.insert(std::array{x, y, z});
        exposed += 6;
        neighbors_for_each([&](auto dx, auto dy, auto dz) {
            if (drops.contains(std::array{x + dx, y + dy, z + dz})) {
                exposed -= 2;
            }
        });
    }
    std::cout << exposed << std::endl;
    return 0;
}
