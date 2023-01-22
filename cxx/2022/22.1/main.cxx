#include <bits/stdc++.h>
#include <range/v3/all.hpp>
#include <boost/assert.hpp>

namespace {
    using ll = long long;
    using ull = unsigned long long;
    using coords_t = std::tuple<std::size_t, std::size_t, int>;
    using map_t = std::vector<std::string>;
}

int main() {
    auto map = std::vector<std::string>{};
    auto max_sz = std::size_t{0};
    {
        auto line = std::string{};
        while (std::getline(std::cin, line) && !line.empty()) {
            max_sz = std::max(max_sz, line.size());
            map.push_back(line);
        }
    }
    auto leftmost = std::vector<std::size_t>{};
    auto rightmost = std::vector<std::size_t>{};
    for (auto &line : map) {
        line.resize(max_sz, ' ');
        leftmost.push_back(line.find_first_not_of(' '));
        rightmost.push_back(line.find_last_not_of(' '));
    }
    auto upmost = std::vector<std::size_t>{};
    auto downmost = std::vector<std::size_t>{};
    for (auto i = std::size_t{0}; i < max_sz; ++i) {
        auto up = std::size_t{0};
        while (up < map.size() && map[up][i] == ' ') {
            ++up;
        }
        upmost.push_back(up);
        auto down = map.size();
        while (down > 0 && map[down-1][i] == ' ') {
            --down;
        }
        downmost.push_back(down-1);
    }
//    for (auto &row : map) {
//        std::cout << row << std::endl;
//    }
    auto pos = coords_t{0, static_cast<int>(map[0].find('.')), 0};
    auto &[y, x, dir] = pos;
    {
        auto steps = 0;
        while (std::cin >> steps) {
            for ([[maybe_unused]] auto i_ : ranges::view::iota(0, steps)) {
                switch (dir) {
                    case 0: {
                        auto next_x = (x + 1) % map[0].size();
                        if (map[y][next_x] == ' ') {
                            next_x = leftmost[y];
                        }
                        if (map[y][next_x] == '.') {
                            x = next_x;
                        }
                        break;
                    }
                    case 1: {
                        auto next_y = (y + 1) % map.size();
                        if (map[next_y][x] == ' ') {
                            next_y = upmost[x];
                        }
                        if (map[next_y][x] == '.') {
                            y = next_y;
                        }
                        break;
                    }
                    case 2: {
                        auto next_x = (x == 0) ? map[0].size() - 1 : x - 1;
                        if (map[y][next_x] == ' ') {
                            next_x = rightmost[y];
                        }
                        if (map[y][next_x] == '.') {
                            x = next_x;
                        }
                        break;
                    }
                    case 3: {
                        auto next_y = (y == 0) ? map.size() - 1 : y - 1;
                        if (map[next_y][x] == ' ') {
                            next_y = downmost[x];
                        }
                        if (map[next_y][x] == '.') {
                            y = next_y;
                        }
                        break;
                    }
                    default:
                        BOOST_ASSERT(false);
                }
            }
            auto direction = char{};
            if (std::cin >> direction) {
                switch (direction) {
                    case 'R':
                    dir = (dir + 1) % 4;
                    break;
                    case 'L':
                    dir = (dir + 3) % 4;
                    break;
                    default:
                        BOOST_ASSERT(false);
                }
            } else {
                break;
            }
        }
        std::cout << (y + 1) * 1000 + (x + 1) * 4 + dir << std::endl;
    }
}
