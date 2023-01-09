#include <bits/stdc++.h>
#include <range/v3/all.hpp>

namespace {
    using coords_t = std::pair<int, int>;
    using filled_t = std::set<coords_t>;
    auto START = std::pair{ 500, 0 };
    auto DUMMY = std::pair{ 0, 0 };
}

std::istream &read_coords(std::istream &is, coords_t& p) {
    char comma;
    return is >> p.first >> comma >> p.second;
}

void fill_edge(filled_t& filled, const coords_t &a, const coords_t &b) {
    if (a.first == b.first) {
        auto [x, y1, y2] = std::tuple{a.first, a.second, b.second};
        if (y1 > y2) { std::swap(y1, y2); }
        for (auto y : ranges::views::iota(y1, y2 + 1)) {
            filled.emplace(x, y);
        }
    } else {
        auto [y, x1, x2] = std::tuple{a.second, a.first, b.first};
        if (x1 > x2) { std::swap(x1, x2); }
        for (auto x : ranges::views::iota(x1, x2 + 1)) {
            filled.emplace(x, y);
        }
    }
}

void fill_path(filled_t& filled, const std::string &path_str) {
    auto iss = std::istringstream{path_str};
    auto prev = coords_t{}, curr = coords_t{};
    auto arrow = std::string{};
    read_coords(iss, prev);
    while (read_coords(iss >> arrow, curr)) {
        fill_edge(filled, prev, curr);
        prev = curr;
    }
}

std::vector<coords_t> &drop_sand(filled_t& filled, const int &above_bedrock, std::vector<coords_t> &path_memo) {
    while (true) {
        auto &[x, y] = path_memo.back();
        if (y == above_bedrock) {
            break;
        }
        auto next = coords_t{x, y + 1};
        if (!filled.contains(next)) {
            path_memo.push_back(next);
            continue;
        }
        --next.first;
        if (!filled.contains(next)) {
            path_memo.push_back(next);
            continue;
        }
        next.first += 2;
        if (!filled.contains(next)) {
            path_memo.push_back(next);
            continue;
        }
        break;
    }
    filled.insert(path_memo.back());
    path_memo.pop_back();
    return path_memo;
}

int main() {
    std::string path_str;

    auto filled = filled_t{};
    while (std::getline(std::cin, path_str)) {
        fill_path(filled, path_str);
    }
    auto above_bedrock = ranges::max(filled | std::views::values) + 1;
    auto path_memo = std::vector<coords_t>{START};
    for (auto i : ranges::views::iota(1)) {
        if (drop_sand(filled, above_bedrock, path_memo).empty()) {
            std::cout << i << std::endl;
            return 0;
        }
    }
}
