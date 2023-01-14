#include <bits/stdc++.h>
#include <range/v3/all.hpp>

namespace {
    using ll = long long;
    using ull = unsigned long long;
    using coords_t = std::pair<ll, ll>;
}

std::istream &extract_coords(std::istream &is, std::map<std::string, std::pair<ll, std::vector<std::string>>> &graph) {
    // Valve
    auto valve = std::string{};
    auto rate = ll{};
    ((is.ignore(6) >> valve)
        //  has flow rate=
        .ignore(15)
        >> rate)
        // ; tunnels lead to valves
        .ignore(25);
    if (!is) {
        return is;
    }
    graph[valve] = {rate, {}};
    auto &[rate_, tunnels] = graph[valve];
    while (is >> valve) {
        valve.resize(2);
    }
}

// Note: the diagnostic.txt and input.txt for 15.1 have a line prepended that denotes the y-coordinate that we are interested in.
int main() {
    std::cout << "occupied" << std::endl;
}
