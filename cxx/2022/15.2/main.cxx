#include <bits/stdc++.h>
#include <range/v3/all.hpp>

namespace {
    using ll = long long;
    using ull = unsigned long long;
    using coords_t = std::pair<ll, ll>;
}

/**
 * A border object denotes a linear border of a region cleared by the sensor, be it
 * the top-left, top-right, bottom-left, or bottom-right line. The coordinates that
 * the line segment passes through are themselves cleared by the sensor (and moving one unit in two
 * out of four directions will bring you to uncleared sections).
 *
 * `axis_point` is the y-coordinate at which the line segment, perhaps extended
 * indefinitely, crosses the y-axis.
 *
 * `start` is the y-coordinate of the leftmost of the two points that bound the line segment, inclusive of the
 * cleared region.
 * `end` is the y-coordinates of the rightmost of the two points that bound the line segment, inclusive of the
 * cleared region.
 * Hence `start` is always less than or equal to `end`.
 */
// Note: x increases to the right, y increases downwards.
struct border {
    ll axis_point;
    ll start;
    ll end;
    auto operator<=>(const border&) const = default;
};

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
    std::vector<border> bl, ul, br, ur, codiags, contradiags;
    while (extract_coords(std::cin, sensor, beacon)) {
        auto &[x, y] = sensor;
        std::cout << "sensor: " << x << ',' << y << std::endl;
        std::cout << "beacon: " << beacon.first << ',' << beacon.second << std::endl;
        beacon.first -= x;
        if (beacon.first < 0) { beacon.first = -beacon.first; }
        beacon.second -= y;
        if (beacon.second < 0) { beacon.second = -beacon.second; }
        const auto distance = beacon.first + beacon.second;
        // note: leftmost cleared is (x - distance, y), rightmost is (x + distance, y)
        //   topmost is (x, y - distance), bottommost (x, y + distance)
        // bl y-intercept is y - (x - distance), ul y-intercept is y + (x - distance)
        // br y-intercept is y + (x + distance), ur y-intercept is y - (x + distance)
        bl.push_back(border{y - x + distance, y, y + distance});
        ul.push_back(border{y + x - distance - 1, y - distance - 1, y - 1});
        ur.push_back(border{y - x - distance - 1, y - distance - 1, y - 1});
        br.push_back(border{y + x + distance, y, y + distance});
    }
    std::ranges::sort(bl);
    std::ranges::sort(ul);
    std::ranges::sort(ur);
    std::ranges::sort(br);

    // find appropriate codiags (these are the lines whose x and y coordinates are covariant).
    std::optional<border> temp;
    for (auto it = bl.begin(), subit = ur.begin(); it != bl.end(), subit != ur.end(); *it < *subit ? ++it : ++subit) {
        if (it->axis_point != subit->axis_point) {
            continue;
        }
        auto overlap = border{it->axis_point, std::max(it->start, subit->start), std::min(it->end, subit->end)};
        if (temp->start > temp->end) {
            // illegal segment
            continue;
        }

    }
    //
}
