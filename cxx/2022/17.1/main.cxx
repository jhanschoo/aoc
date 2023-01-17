#include <bits/stdc++.h>
#include <range/v3/all.hpp>
#include <boost/dynamic_bitset.hpp>

namespace {
    using ll = long long;
    using ull = unsigned long long;
    using row_t = std::bitset<7>;
}

constinit const auto horiz_tet = std::array{
        row_t{0b1111000}
};

constinit const auto cross_tet = std::array{
        row_t{0b0100000},
        row_t{0b1110000},
        row_t{0b0100000}
};

constinit const auto l_tet = std::array{
        row_t{0b1110000},
        row_t{0b0010000},
        row_t{0b0010000}
};

constinit const auto vert_tet = std::array{
        row_t{0b1000000},
        row_t{0b1000000},
        row_t{0b1000000},
        row_t{0b1000000}
};

constinit const auto square_tet = std::array{
        row_t{0b1100000},
        row_t{0b1100000}
};

constinit const auto tets = std::tuple{
        horiz_tet,
        cross_tet,
        l_tet,
        vert_tet,
        square_tet
};

constinit const auto tet_max_shift = std::array{ 3, 4, 4, 6, 5 };

bool overlaps(const auto &tet, const auto &board, const auto &pos) {
    // TODO
}

int main() {
    auto board = std::vector<row_t>{};
    auto lowest_empty = 0ll;
    return 0;
}
