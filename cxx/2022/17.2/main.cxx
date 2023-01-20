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

constinit const auto NUM_TETS = 1000000000000;

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

    auto lowest_empty = std::size_t{0}, virtual_le_delta = std::size_t{0};
    auto tet_num = std::size_t{1}, tet_idx = std::size_t{0}, jet_idx = std::size_t{0};
    /**
     * Assumption: if a tet_idx and jet_idx are visited thrice, such that the difference
     * between the tet_num and lowest_empty of the first and the second visit is the same as the respective difference
     * between the tet_num and lowest_empty of the second and third visit, then we have a cycle.
     */
    auto state_history = std::map<
            std::pair<decltype(tet_idx), decltype(jet_idx)>,
            std::tuple<decltype(tet_num), decltype(tet_num), decltype(lowest_empty), decltype(lowest_empty)>>{};
    auto detect_cycle = [&] {
        if (virtual_le_delta != 0) {
            return;
        }
        auto &[prev_tet_num, prev_tet_delta, prev_le, prev_le_delta] = state_history[{tet_idx, jet_idx}];
        // if first visit
        if (prev_tet_num == 0) {
            prev_tet_num = tet_num;
            prev_le = lowest_empty;
            return;
        }
        // if second visit
        if (prev_tet_delta == 0) {
            prev_tet_delta = tet_num - prev_tet_num;
            prev_tet_num = tet_num;
            prev_le_delta = lowest_empty - prev_le;
            prev_le = lowest_empty;
            return;
        }
        // if third visit
        auto tet_delta = tet_num - prev_tet_num;
        auto le_delta = lowest_empty - prev_le;
        if (tet_delta == prev_tet_delta && le_delta == prev_le_delta) {
            // cycle detected
//            std::cout << "cycle detected1: " << prev_tet_num - tet_delta << '-' << prev_tet_num << '-' << tet_num << ": " << tet_delta << std::endl;
//            std::cout << "cycle detected2: " << prev_le - le_delta << '-' << prev_le << '-' << lowest_empty << ": " << le_delta << std::endl;
            auto multiples = (NUM_TETS - tet_num) / tet_delta;
            tet_num += multiples * tet_delta;
            virtual_le_delta += multiples * le_delta;
        } else {
            prev_tet_num = tet_num;
            prev_tet_delta = tet_delta;
        }
    };
    auto next_jet = [&] {
        auto ret = jet_pattern[jet_idx];
        jet_idx = (jet_idx + 1) % jet_pattern.size();
        return ret;
    };
    auto next_tet = [&](const auto &tet_info) {
        const auto &[max_shift, tet] = tet_info;
        if (tet_num >= NUM_TETS + 1) { return; }
        detect_cycle();

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
//        std::cout << tet_num << ' ' << tet_idx << ' ' << tet_shift << ' ' << tet_pos << ' ' << lowest_empty << ' ' << virtual_le_delta + lowest_empty << '\n';
//        for (const auto &row : ranges::view::reverse(board)) {
//            std::cout << row.to_string('.', '#') << '\n';
//        }
//        std::cout << '\n';

        // increment tet numbers
        ++tet_num;
        tet_idx = (tet_idx + 1) % NUM_DISTINCT_TETS;
    };
    while (tet_num < NUM_TETS + 1) {
        tuple_for_each(tets, next_tet);
    }
    std::cout << virtual_le_delta + lowest_empty << std::endl;
    return 0;
}
