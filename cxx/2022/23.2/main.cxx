#include <bits/stdc++.h>
#include <range/v3/all.hpp>
#include <boost/assert.hpp>

namespace {
    using ll = long long;
    using ull = unsigned long long;
    using coords_t = std::pair<ll, ll>;
    using elves_t = std::set<coords_t>;
}

std::optional<coords_t> get_next_pos(const coords_t &coords, const elves_t &elves, int offset) {
    auto &[y, x] = coords;
    auto neighbors = std::bitset<8>{
            static_cast<ull>(
                    elves.contains({y - 1, x - 1}) |
                    elves.contains({y - 1, x}) << 1 |
                    elves.contains({y - 1, x + 1}) << 2 |
                    elves.contains({y, x - 1}) << 3 |
                    elves.contains({y, x + 1}) << 4 |
                    elves.contains({y + 1, x - 1}) << 5 |
                    elves.contains({y + 1, x}) << 6 |
                    elves.contains({y + 1, x + 1}) << 7
            )
    };
    if (neighbors.none()) {
        return std::nullopt;
    }
    for (char i = 0; i < 4; ++i) {
        switch ((i + offset) % 4) {
            case 0:
                if ((neighbors & std::bitset<8>{"00000111"}).none()) {
                    return coords_t{y - 1, x};
                }
                break;
            case 1:
                if ((neighbors & std::bitset<8>{"11100000"}).none()) {
                    return coords_t{y + 1, x};
                }
                break;
            case 2:
                if ((neighbors & std::bitset<8>{"00101001"}).none()) {
                    return coords_t{y, x - 1};
                }
                break;
            case 3:
                if ((neighbors & std::bitset<8>{"10010100"}).none()) {
                    return coords_t{y, x + 1};
                }
                break;
        }
    }
    return std::nullopt;
}

int main() {
    auto elves = elves_t{};
    {
        auto line = std::string{};
        {
            auto y = 0;
            while (std::getline(std::cin, line) && !line.empty()) {
                for (auto x = 0; x < static_cast<int>(line.size()); ++x) {
                    if (line[x] == '#') {
                        elves.emplace(y, x);
                    }
                }
                ++y;
            }
        }
    }
    for (auto i: ranges::views::iota(0)) {
        auto offset = i % 4;
        auto pos_count = std::map<coords_t, int>{};
        for (const auto &coords: elves) {
            auto next_pos = get_next_pos(coords, elves, offset);
            if (next_pos) {
                ++pos_count[*next_pos];
            }
        }
        auto next_elves = elves_t{};
        bool has_moved = false;
        for (const auto &coords: elves) {
            auto next_pos = get_next_pos(coords, elves, offset);
            if (next_pos && pos_count[*next_pos] == 1) {
                next_elves.insert(*next_pos);
                has_moved = true;
            } else {
                next_elves.insert(coords);
            }
        }
        if (!has_moved) {
            std::cout << i + 1 << std::endl;
            return 0;
        }
        elves = std::move(next_elves);
    }
}
