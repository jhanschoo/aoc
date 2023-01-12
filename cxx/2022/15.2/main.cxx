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

// Note: the diagnostic.txt and input.txt for 15.1 have a line prepended that denotes the y-coordinate that we are interested in.
int main() {
    auto target_y = 0ll;
    std::cin >> target_y;
    std::string path_str;
    auto sensor = coords_t{}, beacon = coords_t{};
    // tycr is target-y row's cleared region, consisting of a map of cleared segments, keyed by start, value is end inclusive.
    auto tycr = std::map<ll, ll>{};
    // tybp is target-y row's positions that contain a beacon. We know that tybps always lie in cleared regions.
    auto tybp = std::set<ll>{};
    while (extract_coords(std::cin, sensor, beacon)) {
        std::cout << "sensor: " << sensor.first << ',' << sensor.second << std::endl;
        std::cout << "beacon: " << beacon.first << ',' << beacon.second << std::endl;
        if (beacon.second == target_y) {
            tybp.insert(beacon.first);
        }
        beacon.first -= sensor.first;
        if (beacon.first < 0) { beacon.first = -beacon.first; }
        beacon.second -= sensor.second;
        if (beacon.second < 0) { beacon.second = -beacon.second; }
        const auto distance = beacon.first + beacon.second;
        const auto distance_y = target_y < sensor.second ? sensor.second - target_y : target_y - sensor.second;
        const auto distance_wrt_y = distance - distance_y;
        if (distance_wrt_y < 0) {
            continue;
        }
        const auto target_y_left_incl = sensor.first - distance_wrt_y;
        const auto target_y_right_incl = sensor.first + distance_wrt_y;
        if (tycr.contains(target_y_left_incl)) {
            tycr[target_y_left_incl] = std::max(tycr[target_y_left_incl], target_y_right_incl);
        } else {
            tycr[target_y_left_incl] = target_y_right_incl;
        }
        std::cout << "d: " << distance << ", d_y: " << distance_y << ", dw_y: " << distance_wrt_y << ", l: " << target_y_left_incl << ", r: " << target_y_right_incl << std::endl;
    }
    if (tycr.empty()) {
        std::cout << 0 << std::endl;
        return 0;
    }
    const auto &prev = *tycr.begin();
    auto occupied = static_cast<ull>(prev.second - prev.first + 1);
    auto prev_right = prev.second;
    for (auto &[start, right] : tycr) {
        auto left = (start <= prev_right) ? prev_right + 1 : start;
        if (left > right) {
            continue;
        }
        occupied += right - left + 1;
        prev_right = right;
    }
    occupied -= tybp.size();
    std::cout << occupied << std::endl;
}
