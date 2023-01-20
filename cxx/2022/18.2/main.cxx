#include <bits/stdc++.h>
#include <range/v3/all.hpp>
#include <boost/dynamic_bitset.hpp>

namespace {
    using ll = long long;
    using ull = unsigned long long;
    using sz = std::size_t;
    using coords_t = std::array<int, 3>;
    using coords_set = std::set<coords_t>;
}

template<typename Func, int ind = 0>
void neighbors_for_each(const Func &f) {
    if constexpr (ind < 27) {
        const auto x = ind % 3 - 1;
        const auto y = ind / 3 % 3 - 1;
        const auto z = ind / 9 % 3 - 1;
        if (x || y || z) {
            f(x, y, z);
        }
        neighbors_for_each<Func, ind + 1>(f);
    }
}

template<typename Func, int ind = 0>
void strict_neighbors_for_each(const Func &f) {
    if constexpr (ind < 27) {
        const auto x = ind % 3 - 1;
        const auto y = ind / 3 % 3 - 1;
        const auto z = ind / 9 % 3 - 1;
        if constexpr ((x!=0) + (y!=0) + (z!=0) == 1) {
            f(x, y, z);
        }
        strict_neighbors_for_each<Func, ind + 1>(f);
    }
}

int main() {
    auto drops = coords_set{};
    auto buffer = char{};
    {
        auto x = 0, y = 0, z = 0;
        while (std::cin >> x >> buffer >> y >> buffer >> z) {
            drops.insert(std::array{x, y, z});
        }
    }
    auto global_neighbors = coords_set{};
    for (auto [x, y, z] : drops) {
        neighbors_for_each([&](auto dx, auto dy, auto dz) {
            auto neighbor = coords_t{x + dx, y + dy, z + dz};
            if (!drops.contains(neighbor)) {
                global_neighbors.insert(neighbor);
            }
        });
    }
    auto components = std::vector<std::pair<int, coords_set>>{};
    while (!global_neighbors.empty()) {
        auto component = coords_set{};
        auto stack = std::vector<coords_t>{};
        auto min_x = int{};
        {
            auto initial = *global_neighbors.begin();
            stack.push_back(initial);
            min_x = initial[0];
        }
        while (!stack.empty()) {
            auto coords = stack.back();
            stack.pop_back();
            auto it = global_neighbors.find(coords);
            if (it == global_neighbors.end()) {
                continue;
            }
            global_neighbors.erase(it);
            min_x = std::min(min_x, coords[0]);
            component.insert(coords);
            auto [x, y, z] = coords;
            strict_neighbors_for_each([&](auto dx, auto dy, auto dz) {
                auto neighbor = coords_t{x + dx, y + dy, z + dz};
                stack.push_back(neighbor);
            });
        }
        components.emplace_back(min_x, std::move(component));
    }
    ranges::sort(components, {}, &std::pair<int, coords_set>::first);
    const auto &exterior = components[0].second;
    auto exterior_surface = 0;
    for (auto [x, y, z] : drops) {
        strict_neighbors_for_each([&](auto dx, auto dy, auto dz) {
            auto neighbor = coords_t{x + dx, y + dy, z + dz};
            if (exterior.contains(neighbor)) {
                ++exterior_surface;
            }
        });
    }
    std::cout << exterior_surface << std::endl;
    return 0;
}
