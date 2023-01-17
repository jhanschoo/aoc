#include <bits/stdc++.h>
#include <range/v3/all.hpp>
#include <boost/dynamic_bitset.hpp>

namespace {
    using ll = long long;
    using ull = unsigned long long;

    constexpr auto MAX_STREAMSIZE = std::numeric_limits<std::streamsize>::max();
    constexpr auto SIZE_ZERO = std::size_t{0};
    constexpr auto num_mins = ll{30};
}

auto parse(std::istream &is) {
    auto buffer = std::string{}, valve_str = std::string{};
    auto neighbors_map = std::map<std::string, std::set<std::string>>{};
    auto rates_map = std::map<std::string, ll>{};
    auto rate_buffer = ll{};
    while (is >> buffer >> valve_str) {
        is.ignore(MAX_STREAMSIZE, '=');
        is >> rate_buffer;
        if (valve_str == "AA" || rate_buffer != 0) {
            rates_map[valve_str] = rate_buffer;
        }
        auto &neighbors = neighbors_map[valve_str];
        is >> buffer >> buffer >> buffer >> buffer >> buffer;
        while (is >> valve_str) {
            valve_str.resize(2);
            neighbors.insert(valve_str);
            if (is.peek() == '\n') {
                break;
            }
            is.ignore(1);
        }
    }
    auto openable_num = rates_map.size();
    auto valves_num = neighbors_map.size();
    auto ident_vec = neighbors_map | ranges::views::keys | ranges::to<std::vector>();
    auto reverse_map = ranges::views::zip(ident_vec, ranges::views::iota(SIZE_ZERO)) |
                       ranges::to<std::map<std::string, ll>>();
    auto neighbors_arr = boost::dynamic_bitset(valves_num * valves_num);
    for (const auto &[valve, neighbors]: neighbors_map) {
        const auto valve_idx = reverse_map[valve];
        for (const auto &neighbor: neighbors) {
            const auto neighbor_idx = reverse_map[neighbor];
            neighbors_arr[valve_idx * valves_num + neighbor_idx] = true;
        }
    }
    auto gaps = std::valarray<ll>(valves_num * valves_num);
    for (auto idx: ranges::views::iota(SIZE_ZERO, valves_num)) {
        auto scheduled = boost::dynamic_bitset(valves_num);
        auto frontier = std::vector<std::size_t>{idx};
        scheduled[idx] = true;
        auto gap = ll{0};
        while (!frontier.empty()) {
            auto new_frontier = decltype(frontier){};
            for (auto dest: frontier) {
                gaps[idx * valves_num + dest] = gaps[dest * valves_num + idx] = gap;
                for (auto next_dest: ranges::views::iota(SIZE_ZERO, valves_num)) {
                    if (scheduled[next_dest]) {
                        continue;
                    }
                    if (!neighbors_arr[dest * valves_num + next_dest]) {
                        continue;
                    }
                    scheduled[next_dest] = true;
                    new_frontier.push_back(next_dest);
                }
            }
            ++gap;
            frontier = new_frontier;
        }
    }
    auto pruned_gaps = decltype(gaps)(openable_num * openable_num);
    auto i = std::size_t{0};
    for (auto idx: ranges::views::iota(SIZE_ZERO, valves_num)) {
        if (!rates_map.contains(ident_vec[idx])) {
            continue;
        }
        auto j = i;
        for (auto jdx: ranges::views::iota(idx, valves_num)) {
            if (!rates_map.contains(ident_vec[jdx])) {
                continue;
            }
            pruned_gaps[i * openable_num + j] = pruned_gaps[j * openable_num + i] = gaps[idx * valves_num + jdx];
            ++j;
        }
        ++i;
    }
    auto rates = rates_map | ranges::view::values | ranges::to<std::vector>();
    return std::pair{rates, pruned_gaps};
}

// Note: the diagnostic.txt and input.txt for 15.1 have a line prepended that denotes the y-coordinate that we are interested in.
int main() {
    auto [rates, gaps] = parse(std::cin);
    auto num_valves = rates.size();
    auto max_rate = std::ranges::max(rates);
    /**
     * Precondition: ts + n < num_mins
     *
     * if you move during [ts, ts + n), you are forfeiting opening the max_rate valve during that period,
     * and forfeiting opening the max_rate valve during [x, x+1) for x=ts, ..., ts + n - 1.
     * if you were to open the max_rate valve at [x, x+1), then the max_rate would release
     * max_rate * (num_mins - (x+1)) amount of gas.
     *
     *   Sum(x=ts..ts+n-1)[max_rate * (num_mins - (x+1))]
     *   = max_rate * [n * {num_mins-(ts+1) + num_mins - (ts+n)})]/2
     */
    auto move_for_n = [&max_rate](auto ts, auto n) {
        return max_rate * (n * ((num_mins + num_mins - 1) - ts - ts - n)) / 2;
    };
    /**
     * opening the valve during [ts, ts + 1). Our distance increases by
     * (max_rate - rate) * (num_mins - ts - 1)
     */
    auto open_at_n = [&max_rate](auto ts, auto rate) {
        return (max_rate - rate) * ((num_mins - 1) - ts);
    };

    auto unbias_distance = [&move_for_n](auto distance) {
        return distance + move_for_n(0, num_mins);
    };

    using state_t = std::tuple<
            ll, // distance
            ll, // timestamp
            std::size_t, // current node
            boost::dynamic_bitset<unsigned long, std::allocator<unsigned long>> // bitmap of all opened valves
    >;
    auto queue = std::priority_queue<state_t>{};
    auto starting_opened = boost::dynamic_bitset{num_valves};
    starting_opened[0] = true;
    queue.emplace(0, 0, 0, starting_opened);
    while (!queue.empty()) {
        auto [distance, ts, valve, opened] = queue.top();
        if (ts == num_mins) {
            std::cout << unbias_distance(distance) << std::endl;
            return 0;
        }
        queue.pop();
        auto has_idle = opened.all();
        if (!opened.all()) {
            for (auto next_valve: ranges::views::iota(SIZE_ZERO + 1, num_valves)) {
                if (opened[next_valve]) {
                    continue;
                }
                auto next_opened = opened;
                next_opened[next_valve] = true;
                auto gap = gaps[valve * num_valves + next_valve];
                auto next_ts = ts + gap + 1;
                // equality case: if we can open the valve at num_mins - 1, choose not to open the valve.
                if (next_ts >= num_mins) {
                    has_idle = true;
                    continue;
                }
                queue.emplace(distance - move_for_n(ts, gap) - open_at_n(ts + gap, rates[next_valve]), next_ts,
                              next_valve, next_opened);
            }
        }
        if (has_idle) {
            queue.emplace(distance - move_for_n(ts, num_mins - ts), num_mins, valve, opened);
        }
    }
}
