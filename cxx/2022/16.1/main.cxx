#include <bits/stdc++.h>
#include <range/v3/all.hpp>

namespace {
    using ll = long long;
    using ull = unsigned long long;
    using ident_t = std::pair<char, char>;
    using raw_graph_t = std::map<ident_t, std::pair<ll, std::vector<ident_t>>>;
    using graph_t = std::map<std::pair<ident_t, ident_t>, ll>;
    using rates_t = std::map<ident_t, ll>;
    using state_t = std::tuple<
            ll, // distance_traveled
            ll, // timestamp
            ident_t, // current valve
            std::vector<ident_t>, // opened valves, vector is efficient for small number of elements
            std::vector<ll> // distance audit
            >;

    constexpr auto MAX_STREAMSIZE = std::numeric_limits<std::streamsize>::max();
    constexpr auto start_valve = ident_t{'A', 'A'};
    constexpr auto num_mins = ll{30};
    constexpr auto final_min = num_mins - 1;
}

std::istream &parse(std::istream &is, raw_graph_t &graph) {
    auto buffer = std::string{};
    auto valve = ident_t{};
    is >> buffer >> valve.first >> valve.second;
    if (!is) {
        return is;
    }
    graph[valve] = {{}, {}};
    auto &[rate, neighbors] = graph[valve];
    is.ignore(MAX_STREAMSIZE, '=');
    is >> rate;
    is >> buffer >> buffer >> buffer >> buffer >> buffer;
    while (is >> valve.first >> valve.second) {
        neighbors.push_back(valve);
        if (is.peek() == '\n') {
            break;
        }
        is.ignore(2);
    }
    return is;
}

std::pair<graph_t, rates_t> process_graph(const raw_graph_t &raw_graph) {
    // bellman-ford
    auto graph = graph_t{};
    auto rates = rates_t{};
    for (const auto &[valve, data] : raw_graph) {
        const auto &[rate, neighbors] = data;
        if (rate != 0) {
            rates[valve] = rate;
        }
        for (const auto &neighbor: neighbors) {
            graph[std::make_pair(valve, neighbor)] = 1;
        }
    }
    while (true) {
        auto new_graph = graph;
        bool changed = false;
        for (const auto &[path, distance]: graph) {
            const auto &[from, intermediate] = path;
            auto it = graph.lower_bound({intermediate, {}});
            while (it != graph.end() && it->first.first == intermediate) {
                const auto &[_, to] = it->first;
                if (from == to) {
                    ++it;
                    continue;
                }
                const auto new_distance = distance + it->second;
                auto new_pair = std::make_pair(from, to);
                if (!new_graph.contains(new_pair) || new_graph[new_pair] > new_distance) {
                    new_graph[new_pair] = new_distance;
                    changed = true;
                }
                ++it;
            }
        }
        if (!changed) {
            break;
        }
        graph = new_graph;
    }

    auto pruned_graph = graph_t{};
    for (const auto &[path, distance]: graph) {
        const auto &[from, to] = path;
        if ((rates.contains(from) || from == start_valve) && rates.contains(to)) {
            pruned_graph[path] = distance;
        }
    }
    return {pruned_graph, rates};
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

/**
 *
 * @param graph
 * @param to_open
 * @param ts
 * @param max_rate
 * @return a nonpositive distance for use with std::priority_queue; the less the gas released, the lower the distance
 */
auto get_distance(const raw_graph_t &graph, const std::optional<ident_t> &to_open, const ll &ts, const ll &max_rate, const ll &gap) {
    if (ts + gap >= final_min) {
        return wait_to_final(graph, ts, max_rate);
    }
    return (to_open ? get_gas_released(graph, *to_open, ts) : 0) - max_rate * static_cast<ll>(final_min - ts);
}

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
    auto raw_graph = raw_graph_t{};
    while (parse(std::cin, raw_graph)) { }
    auto [graph, rates] = process_graph(raw_graph);
    for (const auto &[edge, distance] : graph) {
        std::cout << edge.first.first << edge.first.second << " -> " << edge.second.first << edge.second.second << ": " << distance << '\n';
    }
    auto max_rate = std::ranges::max(rates | ranges::views::values);
    std::cout << max_rate << std::endl;
    auto queue = std::priority_queue<state_t>{};
    queue.push({0, 0, start_valve, {}, {}});
    while (!queue.empty()) {
        std::cout << "size: " << queue.size() << std::endl;
        auto [distance_traveled, ts, current_valve, opened_valves, distance_audit] = queue.top();
        if (ts >= num_mins) {
            break;
        }
        queue.pop();
        opened_valves.push_back(current_valve);
        distance_audit.push_back(distance_traveled);
        // choice where we open current valve
        if (!ranges::contains(opened_valves, current_valve) && graph.at(current_valve).first != 0) {
            queue.emplace(distance_traveled + get_distance(graph, current_valve, ts, max_rate), ts + 1, current_valve, opened_valves, distance_audit);
        }
        // TODO: optimization where we no-op if we have already opened all valves
        // choice where we move to next valve
        for (const auto &next_valve : graph.at(current_valve).second) {
            queue.emplace(distance_traveled + get_distance(graph, std::nullopt, ts, max_rate), ts + 1, next_valve, opened_valves, distance_audit);
        }
    }
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
