#include <bits/stdc++.h>
#include <range/v3/all.hpp>
#include <boost/dynamic_bitset.hpp>

namespace {
    using ll = long long;
    using ull = unsigned long long;
    using row_t = std::bitset<7>;
}

constinit const auto tets = std::tuple{
        std::pair{std::size_t{3}, std::array{
                row_t{0b1111000}
        }},
        std::pair{std::size_t{4}, std::array{
                row_t{0b0100000},
                row_t{0b1110000},
                row_t{0b0100000}
        }},
        std::pair{std::size_t{4}, std::array{
                row_t{0b1110000},
                row_t{0b0010000},
                row_t{0b0010000}
        }},
        std::pair{std::size_t{6}, std::array{
                row_t{0b1000000},
                row_t{0b1000000},
                row_t{0b1000000},
                row_t{0b1000000}
        }},
        std::pair{std::size_t{5}, std::array{
                row_t{0b1100000},
                row_t{0b1100000}
        }},
};

constinit const auto NUM_DISTINCT_TETS = std::tuple_size<decltype(tets)>::value;

constinit const auto tet_max_shift = std::array<int, NUM_DISTINCT_TETS>{3, 4, 4, 6, 5};

constinit const auto NUM_TETS = 2022;

constinit const auto GAP = std::size_t{3};

template<size_t Index, typename Tuple, typename Func>
void tuple_at(const Tuple &t, Func f) {
    const auto &v = std::get<Index>(t);
    std::invoke(f, v);
}

template<typename Tuple, typename Func, size_t Index = 0>
void tuple_for_each(const Tuple &t, const Func &f) {
    constexpr auto n = std::tuple_size_v<Tuple>;
    if constexpr (Index < n) {
        tuple_at<Index>(t, f);
        tuple_for_each<Tuple, Func, Index + 1>(t, f);
    }
}

int main() {
    auto board = std::vector<row_t>{};
    auto jet_pattern = std::string{};
    std::cin >> jet_pattern;

    auto lowest_empty = std::size_t{0};
    auto tet_num = std::size_t{0}, tet_idx = std::size_t{0}, jet_idx = std::size_t{0};
    auto next_jet = [&] {
        auto ret = jet_pattern[jet_idx];
        jet_idx = (jet_idx + 1) % jet_pattern.size();
        return ret;
    };
    auto next_tet = [&](const auto &tet_info) {
        const auto &[max_shift, tet] = tet_info;
        if (tet_num >= NUM_TETS) { return; }

        board.resize(lowest_empty + GAP + tet.size());
        auto tet_shift = std::size_t{2};
        auto tet_pos = lowest_empty + GAP;
        auto optimistically_update_shift = [&] {
            switch(next_jet()) {
                case '<':
                    (tet_shift > 0) && (--tet_shift);
                    break;
                case '>':
                    (tet_shift < max_shift) && (++tet_shift);
                    break;
                default:
                    break;
            }
        };
        auto optimistically_update_pos = [&] { --tet_pos; };
        for (auto idx = std::size_t{0}; idx < GAP; ++idx) {
            optimistically_update_shift();
            optimistically_update_pos();
        }
        auto overlap_fn = [&]() {
            auto row_overlaps = ranges::view::zip(tet, board | ranges::view::drop(tet_pos));
            return ranges::any_of(row_overlaps, [&tet_shift](const auto &p) {
                const auto &[tet_row, board_row] = p;
                return ((tet_row >> tet_shift) & board_row).any();
            });
        };
        while (true) {
            auto old_shift = tet_shift;
            optimistically_update_shift();
            if (tet_shift != old_shift && overlap_fn()) {
                tet_shift = old_shift;
            }
            if (tet_pos == 0) {
                break;
            } else {
                optimistically_update_pos();
                if (overlap_fn()) {
                    ++tet_pos;
                    break;
                }
            }
        }

        // Place tet
        for (auto idx = std::size_t{0}; idx < tet.size(); ++idx) {
            board[tet_pos + idx] |= (tet[idx] >> tet_shift);
        }
        lowest_empty = std::max(lowest_empty, tet_pos + tet.size());

        // increment tet numbers
        ++tet_num;
        tet_idx = (tet_idx + 1) % NUM_DISTINCT_TETS;
    };
    while (tet_num < NUM_TETS) {
        tuple_for_each(tets, next_tet);
    }
    std::cout << lowest_empty << std::endl;
    return 0;
}
