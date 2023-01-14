#include <bits/stdc++.h>
#include <range/v3/all.hpp>

namespace {
    using ll = long long;
    using ull = unsigned long long;
    using coords_t = std::pair<ll, ll>;
}

std::istream &extract_coords(std::istream &is, coords_t &sensor, coords_t &beacon) {
    // \nSensor at x=
    return (((is.ignore(13) >> sensor.first)
    // , y=
            .ignore(4)
            >> sensor.second)
            // : closest beacon is at x=
            .ignore(25)
            >> beacon.first)
            // , y=
            .ignore(4)
            >> beacon.second;
}

void normalize_borders(std::map<ll, std::vector<coords_t>> &borders_map) {
    for (auto &[axis_point, borders] : borders_map) {
        ranges::sort(borders);
        std::vector<coords_t> new_borders;
        for (const auto &border : borders) {
            if (new_borders.empty()) {
                new_borders.push_back(border);
                continue;
            }
            auto &last_border = new_borders.back();
            if (last_border.second < border.first) {
                new_borders.push_back(border);
                continue;
            }
            last_border.second = std::max(last_border.second, border.second);
        }
        borders = std::move(new_borders);
    }
}

std::map<ll, std::set<coords_t>> borders_intersection(const std::map<ll, std::vector<coords_t>> &b1, const std::map<ll, std::vector<coords_t>> &b2) {
    auto result = std::map<ll, std::set<coords_t>>{};
    for (auto it1 = b1.begin(), it2 = b2.begin(); it1 != b1.end() && it2 != b2.end();) {
        auto &[axis_point1, borders1] = *it1;
        auto &[axis_point2, borders2] = *it2;
        if (axis_point1 < axis_point2) {
            ++it1;
            continue;
        }
        if (axis_point2 < axis_point1) {
            ++it2;
            continue;
        }
        for (auto bit1 = borders1.begin(), bit2 = borders2.begin(); bit1 != borders1.end() && bit2 != borders2.end();) {
            auto &[start1, end1] = *bit1;
            auto &[start2, end2] = *bit2;
            if (end1 < start2) {
                ++bit1;
                continue;
            }
            if (end2 < start1) {
                ++bit2;
                continue;
            }
            result[axis_point1].emplace(std::max(start1, start2), std::min(end1, end2));
            if (end1 < end2) {
                ++bit1;
            } else {
                ++bit2;
            }
        }
        ++it1;
        ++it2;
    }
    return result;
}

void print_borders(const std::map<ll, std::vector<coords_t>> &borders_map) {
    for (const auto &[axis_point, borders] : borders_map) {
        std::cout << axis_point << ": ";
        for (const auto &[start, end] : borders) {
            std::cout << '[' << start << ", " << end << "] ";
        }
        std::cout << std::endl;
    }
}

void print_diags(const std::map<ll, std::set<coords_t>> &diags) {
    for (const auto &[axis_point, borders] : diags) {
        for (const auto &[start, end] : borders) {
            std::cout << "y-intercept y=" << axis_point << ": y=" << start << " to y=" << end << '\n';
        }
    }
}

// Note: the diagnostic.txt and input.txt for 15.1 have a line prepended that denotes the y-coordinate that we are interested in.
int main() {
    auto target_y = 0ll;
    std::cin >> target_y;
    std::string path_str;
    auto sensor = coords_t{}, beacon = coords_t{};
    std::array<std::map<ll, std::vector<coords_t>>, 4> borders_maps;
    auto &[bl, br, ul, ur] = borders_maps;
    while (extract_coords(std::cin, sensor, beacon)) {
        auto &[x, y] = sensor;
        beacon.first -= x;
        if (beacon.first < 0) { beacon.first = -beacon.first; }
        beacon.second -= y;
        if (beacon.second < 0) { beacon.second = -beacon.second; }
        const auto distance = beacon.first + beacon.second;
        // note: leftmost cleared is (x - distance, y), rightmost is (x + distance, y)
        //   topmost is (x, y - distance), bottommost (x, y + distance)
        // bl y-intercept is y - (x - distance), ul y-intercept is y + (x - distance)
        // br y-intercept is y + (x + distance), ur y-intercept is y - (x + distance)
//        std::cout << "Sensor at (" << x << ", " << y << ") sees beacon at distance " << distance << '\n';
        bl[y - x + distance + 1].emplace_back(y + 1, y + distance + 1);
        ul[y + x - distance - 1].emplace_back(y - distance - 1, y - 1);
        ur[y - x - distance - 1].emplace_back(y - distance - 1, y - 1);
        br[y + x + distance + 1].emplace_back(y + 1, y + distance + 1);
    }
    for (auto &border_map : borders_maps) {
        normalize_borders(border_map);
    }

    // find appropriate codiags (these are the lines whose x and y coordinates are covariant).
    auto codiags = borders_intersection(bl, ur), contradiags = borders_intersection(ul, br);
    for (auto &[cod_y, borders] : codiags) {
        if (borders.empty()) { continue; }
        for (auto &[ctr_y, ctr_borders] : contradiags) {
            if (ctr_borders.empty()) { continue; }
            // coordinates are positive, and distance must be even
            if (cod_y > ctr_y || (ctr_y - cod_y) % 2 != 0) {
                continue;
            }
            auto y = cod_y + (ctr_y - cod_y) / 2;
            auto bit = borders.upper_bound({y, 0});
            if (bit == borders.begin()) { continue; }
            bit--;
            if (bit->second < y) { continue; }
            auto ctr_bit = ctr_borders.upper_bound({y, 0});
            if (ctr_bit == ctr_borders.begin()) { continue; }
            ctr_bit--;
            if (ctr_bit->second < y) { continue; }
            std::cout << (y - cod_y) * 4000000 + y << std::endl;
        }
    }
}
