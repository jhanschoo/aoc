#include <bits/stdc++.h>
#include <range/v3/all.hpp>
#include <boost/assert.hpp>
#include <boost/multi_index_container.hpp>
//#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/ranked_index.hpp>
#include <boost/multi_index/key_extractors.hpp>

struct number_t {
    std::size_t orig_idx;
    long long value;
    long long true_value;
    bool dummy;
};

namespace {
    using ll = long long;
    using ull = unsigned long long;
    // std::size_t indicates the original index of the number
    using numbers_list_t = boost::multi_index_container<
            number_t,
            boost::multi_index::indexed_by<
                    boost::multi_index::ordered_unique<boost::multi_index::member<number_t, decltype(number_t::orig_idx), &number_t::orig_idx>>,
                    boost::multi_index::ranked_non_unique<boost::multi_index::member<number_t, decltype(number_t::dummy), &number_t::dummy>>
            >
    >;
    const auto decryption_key = 811589153ll;
}

auto relocate(numbers_list_t &numbers, std::size_t from, std::size_t to) {
    auto &nums_rank_idx = numbers.get<1>();
    if (to > from) {
        ++to;
    }
    auto it_from = nums_rank_idx.nth(from), it_to = nums_rank_idx.nth(to);
    return nums_rank_idx.insert(it_to, nums_rank_idx.extract(it_from));
}

//void circular_shift_down(numbers_list_t &numbers, std::size_t shift) {
//    shift %= numbers.size();
//    auto &nums_rank_idx = numbers.get<1>();
//    for (auto i_ : ranges::views::iota(std::size_t{0}, shift)) {
//        auto it_from = nums_rank_idx.begin(), it_to = nums_rank_idx.end();
//        nums_rank_idx.insert(it_to, nums_rank_idx.extract(it_from));
//    }
//}
//
//void circular_shift_up(numbers_list_t &numbers, std::size_t shift) {
//    shift %= numbers.size();
//    auto &nums_rank_idx = numbers.get<1>();
//    for (auto i_ : ranges::views::iota(std::size_t{0}, shift)) {
//        auto it_from = nums_rank_idx.end(), it_to = nums_rank_idx.begin();
//        --it_from;
//        nums_rank_idx.insert(it_to, nums_rank_idx.extract(it_from));
//    }
//}

int main() {

    auto numbers = numbers_list_t{};
    auto &nums_orig_idx = numbers.get<0>();
    auto &nums_rank_idx = numbers.get<1>();
    auto zero_orig_idx = std::size_t{0};
    auto orig_numbers = ranges::views::zip(
            ranges::views::iota(std::size_t{0}),
            ranges::istream_view<int>(std::cin),
            ranges::views::repeat(false)
    ) | ranges::views::transform([&zero_orig_idx](auto &&tup) {
        auto &&[idx, val, dummy] = tup;
        if (val == 0) {
            zero_orig_idx = idx;
        }
        return number_t{idx, val * decryption_key, val * decryption_key, dummy};
    }) | ranges::to<std::vector>();
    auto sg_numbers_sz = static_cast<ll>(orig_numbers.size());
    for (auto &num : orig_numbers) {
        num.value %= sg_numbers_sz - 1;
    }
    for (auto &&num: orig_numbers) {
        nums_orig_idx.emplace(num);
    }
    for (auto i_ : ranges::views::iota(0, 10)) {
        for (auto &[orig_idx, raw_value, true_value_, dummy_]: orig_numbers) {
            if (raw_value == 0) {
                continue;
            }
            auto value = raw_value;
            // perform trivial shifts
//        auto shifts = value / (sg_numbers_sz - 1);
            // not necessary for now
//        if (shifts > 0) {
//            circular_shift_down(numbers, shifts);
//        } else if (shifts < 0) {
//            circular_shift_up(numbers, -shifts);
//        }
//        value %= sg_numbers_sz - 1;

            // reinsert number
            auto it_orig = nums_orig_idx.find(orig_idx);
            auto it_rank = numbers.project<1>(it_orig);
            auto from_rank = nums_rank_idx.rank(it_rank);
            // handle past-the-end case
            if (value > 0 && from_rank + value > sg_numbers_sz - 1) {
                // past the rightmost end
//            circular_shift_down(numbers, 1);
//            --from_rank;
                value = sg_numbers_sz - 1 - value;
                auto to_rank = from_rank - value;
                relocate(numbers, from_rank, to_rank);
            } else if (value < 0 && from_rank < 0 - value) {
                // past the leftmost end
//            circular_shift_up(numbers, 1);
//            ++from_rank;
                value = sg_numbers_sz - 1 + value;
                auto to_rank = from_rank + value;
                relocate(numbers, from_rank, to_rank);
            } else {
                auto to_rank = from_rank + value;
                relocate(numbers, from_rank, to_rank);
            }
//        for (auto &[o_, v, b_] : nums_rank_idx) {
//            std::cout << v << ' ';
//        }
//        std::cout << std::endl;
        }
    }
    // find grove coords
    auto it_orig = nums_orig_idx.find(zero_orig_idx);
    auto it_rank = numbers.project<1>(it_orig);
    auto zero_rank = nums_rank_idx.rank(it_rank);
    auto sum = 0ll;
    for (auto delta : {1000, 2000, 3000}) {
        sum += nums_rank_idx.nth((zero_rank + delta) % numbers.size())->true_value;
    }
    std::cout << sum << std::endl;
}
