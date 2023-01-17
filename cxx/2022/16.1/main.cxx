#include <bits/stdc++.h>
#include <range/v3/all.hpp>
#include <boost/dynamic_bitset.hpp>

namespace {
    using ll = long long;
    using ull = unsigned long long;
    using ident_t = std::pair<char, char>;
    using raw_graph_t = std::map<ident_t, std::pair<ll, std::vector<ident_t>>>;

    constexpr auto MAX_STREAMSIZE = std::numeric_limits<std::streamsize>::max();
    constexpr auto num_mins = ll{30};
    constexpr auto final_min = num_mins - 1;
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
    auto reverse_map = ranges::views::zip(ident_vec, ranges::views::iota(static_cast<std::size_t>(0))) | ranges::to<std::map<std::string, ll>>();
    auto neighbors_arr = boost::dynamic_bitset(valves_num * valves_num);
    for (const auto &[valve, neighbors] : neighbors_map) {
        const auto valve_idx = reverse_map[valve];
        for (const auto &neighbor : neighbors) {
            const auto neighbor_idx = reverse_map[neighbor];
            neighbors_arr[valve_idx * valves_num + neighbor_idx] = true;
        }
    }
    auto gaps = std::valarray<ll>(valves_num * valves_num);
    for (auto idx : ranges::views::iota(static_cast<std::size_t>(0), valves_num)) {
        auto scheduled = boost::dynamic_bitset(valves_num);
        auto frontier = std::vector<std::size_t>{idx};
        scheduled[idx] = true;
        auto gap = ll{0};
        while (!frontier.empty()) {
            auto new_frontier = decltype(frontier){};
            for (auto dest : frontier) {
                gaps[idx * valves_num + dest] = gaps[dest * valves_num + idx] = gap;
                for (auto next_dest : ranges::views::iota(static_cast<std::size_t>(0), valves_num)) {
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
    for (auto idx : ranges::views::iota(static_cast<std::size_t>(0), valves_num)) {
        if (!rates_map.contains(ident_vec[idx])) {
            continue;
        }
        auto j = i;
        for (auto jdx : ranges::views::iota(idx, valves_num)) {
            if (!rates_map.contains(ident_vec[jdx])) {
                continue;
            }
            pruned_gaps[i * openable_num + j] = gaps[idx * valves_num + jdx];
            pruned_gaps[j * openable_num + i] = gaps[idx * valves_num + jdx];
            ++j;
        }
        ++i;
    }
    for (auto idx : ranges::views::iota(static_cast<std::size_t>(0), openable_num)) {
        std::cout << idx << ": ";
        for (auto jdx : ranges::views::iota(static_cast<std::size_t>(0), openable_num)) {
            std::cout << pruned_gaps[idx * openable_num + jdx] << ' ';
        }
        std::cout << std::endl;
    }
    auto rates = rates_map | ranges::view::values | ranges::to<std::vector>();
    for (auto idx : ranges::views::iota(static_cast<std::size_t>(0), openable_num)) {
        std::cout << idx << ": " << rates[idx] << std::endl;
    }
    return std::pair{rates, pruned_gaps};
}

auto wait_time(const ll &ts, const ll &max_rate, const ll &gap) {
    return max_rate * (gap * (final_min - ts) + gap * (gap - 1) / 2);
}

/**
 * @param graph
 * @param to_open
 * @param ts the zero-based timestamp of the current time.
 *      In the example, move_to(DD) during t=0, open(DD) during t=1, hence amortized gas is released for 29-1=28 minutes.
 *      If we are opening a valve during t=29=final_min, then we release gas for 0 minutes.
 * @return the amortized gas released by opening the `to_open` valve at `ts`.
 */
auto get_gas_released(const raw_graph_t &graph, const ident_t &to_open, const ll ts) {
    return graph.at(to_open).first * static_cast<ll>(final_min - ts);
}

///**
// *
// * @param graph
// * @param to_open
// * @param ts
// * @param max_rate
// * @return a nonpositive distance for use with std::priority_queue; the less the gas released, the lower the distance
// */
//auto get_distance(const raw_graph_t &graph, const std::optional<ident_t> &to_open, const ll &ts, const ll &max_rate, const ll &gap) {
//    if (ts + gap >= final_min) {
//        return wait_to_final(graph, ts, max_rate);
//    }
//    return (to_open ? get_gas_released(graph, *to_open, ts) : 0) - max_rate * static_cast<ll>(final_min - ts);
//}

auto unbias_distance(ll max_rate, ll distance) {
    for (auto ts : ranges::views::iota(ll{0}, static_cast<ll>(final_min))) {
        distance += max_rate * static_cast<ll>(final_min - ts);
    }
    return distance;
}

void print_graph(const raw_graph_t &graph) {
    for (const auto &[valve, data] : graph) {
        std::cout << valve.first << valve.second << " (" << data.first << "): ";
        for (const auto &next : data.second) {
            std::cout << next.first << next.second << ", ";
        }
        std::cout << '\n';
    }
}

// Note: the diagnostic.txt and input.txt for 15.1 have a line prepended that denotes the y-coordinate that we are interested in.
int main() {
    auto [rates, gaps] = parse(std::cin);
    auto max_rate = std::ranges::max(rates);
    auto move_for_n = [&max_rate](auto ts, auto n) {
        // if you move during [ts, ts + n), you are forfeiting opening the max_rate valve during that period,
        // and forfeiting opening the max_rate valve during [x, x+1) for x=ts, ..., ts + n - 1.
        // if you were to open the max_rate valve at [x, x+1), then the max_rate would release
        // max_rate * (num_mins - (x+1)) amount of gas.
        // Sum(x=ts..ts+n-1)[max_rate * (num_mins - (x+1))]
        // = max_rate * [n * {num_mins-(ts+1) + num_mins - (ts+n)})]/2
        return max_rate * (n * (num_mins - (ts + 1) + num_mins - (ts + n)) / 2);
    };
//    std::cout << max_rate << std::endl;
//    auto queue = std::priority_queue<state_t>{};
//    queue.push({0, 0, start_valve, {}, {}});
//    while (!queue.empty()) {
//        std::cout << "size: " << queue.size() << std::endl;
//        auto [distance_traveled, ts, current_valve, opened_valves, distance_audit] = queue.top();
//        if (ts >= num_mins) {
//            break;
//        }
//        queue.pop();
//        opened_valves.push_back(current_valve);
//        distance_audit.push_back(distance_traveled);
//        // choice where we open current valve
//        if (!ranges::contains(opened_valves, current_valve) && graph.at(current_valve).first != 0) {
//            queue.emplace(distance_traveled + get_distance(graph, current_valve, ts, max_rate), ts + 1, current_valve, opened_valves, distance_audit);
//        }
//        // TODO: optimization where we no-op if we have already opened all valves
//        // choice where we move to next valve
//        for (const auto &next_valve : graph.at(current_valve).second) {
//            queue.emplace(distance_traveled + get_distance(graph, std::nullopt, ts, max_rate), ts + 1, next_valve, opened_valves, distance_audit);
//        }
//    }
//    while (!queue.empty()) {
//        auto [distance_traveled, ts, current_valve, opened_valves, distance_audit] = queue.top();
//        queue.pop();
//        std::cout << "dist: " << distance_traveled << ", ts: " << ts << std::endl;
//        for (auto &[c1, c2] : opened_valves) {
//            std::cout << c1 << c2 << ", ";
//        }
//        std::cout << std::endl;
//        for (auto &dist : distance_audit) {
//            std::cout << dist << ", ";
//        }
//        std::cout << std::endl;
//
//    }
}
