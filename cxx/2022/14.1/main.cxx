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

//std::vector<std::pair<int, int>> read_path(const std::string &path_str) {
//    auto iss = std::istringstream{path_str};
//    auto path = std::vector<std::pair<int, int>>{};
//    // first
//    path.emplace_back(0, 0);
//    read_coords(iss, path.back());
//    auto arrow = std::string{};
//    while (iss >> arrow) {
//        path.emplace_back(0, 0);
//        read_coords(iss, path.back());
//    }
//    return path;
//}

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

void print_filled(filled_t& filled) {
    auto [min_x, max_x] = ranges::minmax(filled | std::views::keys);
    auto [min_y, max_y] = ranges::minmax(filled | std::views::values);
    for (auto y : ranges::views::iota(min_y, max_y + 1)) {
        for (auto x : ranges::views::iota(min_x, max_x + 1)) {
            std::cout << (filled.contains({x, y}) ? '#' : '.');
        }
        std::cout << std::endl;
    }
}

void print_coords(const coords_t &p) {
    std::cout << p.first << ',' << p.second;
}

std::optional<coords_t> drop_vertical(const filled_t& filled, const coords_t &current) {
    auto below = filled.lower_bound(current);
    if (below->first != current.first) {
        return std::nullopt;
    }
    if (below == filled.end() || below->first != current.first) {
        return std::nullopt;
    }
    return *below;
}

bool drop_sand(filled_t& filled, std::vector<coords_t> &path_memo) {
    coords_t prev{DUMMY}, curr;
    while (filled.contains(curr = path_memo.back())) {
        path_memo.pop_back();
    }
    while (curr != prev) {
        prev = curr;
        auto res = drop_vertical(filled, curr);
        if (!res) {
            return false;
        }
        curr = *res;
        --curr.second;
        path_memo.push_back(curr);
        auto diag_left = std::pair{curr.first - 1, curr.second + 1};
        if (!filled.contains(diag_left)) {
            curr = diag_left;
            path_memo.push_back(curr);
            continue;
        }
        auto diag_right = std::pair{curr.first + 1, curr.second + 1};
        if (!filled.contains(diag_right)) {
            curr = diag_right;
            path_memo.push_back(curr);
        }
    }
    filled.insert(curr);
    return true;
}

int main() {
    std::string path_str;

    auto filled = filled_t{};
    while (std::getline(std::cin, path_str)) {
        fill_path(filled, path_str);
    }
    auto path_memo = std::vector<coords_t>{START};
    for (auto i : ranges::views::iota(0)) {
        if (!drop_sand(filled, path_memo)) {
            std::cout << i << std::endl;
            return 0;
        }
    }
}
