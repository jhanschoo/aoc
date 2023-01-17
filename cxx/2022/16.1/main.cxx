#include <bits/stdc++.h>
#include <range/v3/all.hpp>
#include <boost/dynamic_bitset.hpp>

namespace {
    using ll = long long;
    using ull = unsigned long long;
    using dbitset = boost::dynamic_bitset<unsigned long, std::allocator<unsigned long>>;
    // bool denotes if we're traversing down or up the DFS
    using state_t = std::pair<std::size_t, bool>;

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
    auto neighbors_arr = dbitset(valves_num * valves_num);
    for (const auto &[valve, neighbors]: neighbors_map) {
        const auto valve_idx = reverse_map[valve];
        for (const auto &neighbor: neighbors) {
            const auto neighbor_idx = reverse_map[neighbor];
            neighbors_arr[valve_idx * valves_num + neighbor_idx] = true;
        }
    }
    auto gaps = std::valarray<ll>(valves_num * valves_num);
    for (auto idx: ranges::views::iota(SIZE_ZERO, valves_num)) {
        auto scheduled = dbitset(valves_num);
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

auto open_at_ts_minus_1(auto ts, auto rate) {
    return rate * (num_mins - ts);
}

int main() {
    auto [rates, gaps] = parse(std::cin);
    auto num_valves = rates.size();
    /**
     * opening the valve during [ts - 1, ts). Our flow increases by
     * rate * (num_mins - ts)
     */
    auto unvisited = ~dbitset{num_valves};
    unvisited[0] = false;
    auto ts = ll{0}, max_flow = ll{0}, flow = ll{0};
    auto last_visited = std::size_t{0};
    auto path = std::vector<std::size_t>{0};
    while (true) {
        decltype(unvisited.find_first()) next;
        // Handle initial arrival on this node
        if (last_visited == path.back()) {
            // Update max flow
            if (flow > max_flow) {
                max_flow = flow;
            }
//            for (auto idx: path) {
//                std::cout << idx << ' ';
//            }
//            std::cout << flow << std::endl;
            next = unvisited.find_first();
        } else {
            next = unvisited.find_next(last_visited);
        }
        while (next != dbitset::npos && ts + gaps[path.back() * num_valves + next] + 1 >= num_mins) {
            next = unvisited.find_next(next);
        }
        // Handle traversing up the DFS
        if (next == dbitset::npos) {
            unvisited[path.back()] = true;
            flow -= open_at_ts_minus_1(ts, rates[path.back()]);
            last_visited = path.back();
            path.pop_back();
            if (path.empty()) {
                break;
            }
            ts -= gaps[path.back() * num_valves + last_visited] + 1;
            continue;
            // Handle traversing down the DFS
        } else {
            ts = ts + gaps[path.back() * num_valves + next] + 1;
            path.push_back(next);
            last_visited = next;
            flow += open_at_ts_minus_1(ts, rates[next]);
            unvisited[next] = false;
        }
    }
    std::cout << max_flow << std::endl;
}
