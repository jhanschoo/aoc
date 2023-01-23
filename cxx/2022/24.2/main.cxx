#include <bits/stdc++.h>
#include <range/v3/all.hpp>
#include <boost/assert.hpp>
#include <boost/dynamic_bitset.hpp>

namespace {
    using ll = long long;
    using ull = unsigned long long;
    using coords_t = std::pair<ll, ll>;
}

inline auto urot(const std::size_t &chunk_sz, const boost::dynamic_bitset<> &bs) {
    return bs >> chunk_sz | bs << (bs.size() - chunk_sz);
}

inline auto ushift(const std::size_t &chunk_sz, const boost::dynamic_bitset<> &bs) {
    return bs >> chunk_sz;
}

inline auto drot(const std::size_t &chunk_sz, const boost::dynamic_bitset<> &bs) {
    return bs << chunk_sz | bs >> (bs.size() - chunk_sz);
}

inline auto dshift(const std::size_t &chunk_sz, const boost::dynamic_bitset<> &bs) {
    return bs << chunk_sz;
}

// note: lrot, rrot are chunked cyclic right and left shifts respectively (left/right convention
//   is different from bitsets)
inline auto lrot(const std::size_t &chunk_sz, const boost::dynamic_bitset<> &bs, const boost::dynamic_bitset<> &chunk_mask) {
//    auto lshifted = bs >> 1;
//    auto overflowed_bits = bs & chunk_mask;
//    auto overflowed_bits_relocated = overflowed_bits << (chunk_sz - 1);
//    auto lshifted_masked = lshifted & ~(chunk_mask << (chunk_sz - 1));
//    return lshifted_masked | overflowed_bits_relocated;
    return (bs >> 1 & ~(chunk_mask << (chunk_sz - 1))) | (bs & chunk_mask) << (chunk_sz - 1);
}

inline auto lshift(const std::size_t &chunk_sz, const boost::dynamic_bitset<> &bs, const boost::dynamic_bitset<> &chunk_mask) {
    return (bs >> 1) & ~(chunk_mask << (chunk_sz - 1));
}

// note: lrot, rrot are chunked cyclic right and left shifts respectively (left/right convention
//   is different from bitsets)
inline auto rrot(const std::size_t &chunk_sz, const boost::dynamic_bitset<> &bs, const boost::dynamic_bitset<> &chunk_mask) {
//    auto rshifted = bs << 1;
//    auto overflowed_bits = bs & (chunk_mask << (chunk_sz - 1));
//    auto overflowed_bits_relocated = overflowed_bits >> (chunk_sz - 1);
//    auto rshifted_masked = rshifted & ~chunk_mask;
//    return rshifted_masked | overflowed_bits_relocated;
    return (bs << 1 & ~chunk_mask) | (bs & (chunk_mask << (chunk_sz - 1))) >> (chunk_sz - 1);
}

inline auto rshift([[maybe_unused]] const std::size_t &chunk_sz, const boost::dynamic_bitset<> &bs, const boost::dynamic_bitset<> &chunk_mask) {
    return (bs << 1) & ~chunk_mask;
}

inline void step_blizzards(auto &u_blizz, auto &d_blizz, auto &l_blizz, auto &r_blizz, const auto &chunk_sz, const auto &chunk_mask) {
    u_blizz = urot(chunk_sz, u_blizz);
    d_blizz = drot(chunk_sz, d_blizz);
    l_blizz = lrot(chunk_sz, l_blizz, chunk_mask);
    r_blizz = rrot(chunk_sz, r_blizz, chunk_mask);
}

inline void optimistic_step_positions_fwd(auto &positions, const auto &chunk_sz, const auto &chunk_mask) {
    positions = (positions
            | ushift(chunk_sz, positions)
            | dshift(chunk_sz, positions)
            | lshift(chunk_sz, positions, chunk_mask)
            | rshift(chunk_sz, positions, chunk_mask)).set(0);
}

inline void optimistic_step_positions_bkwd(auto &positions, const auto &chunk_sz, const auto &chunk_mask) {
    positions = (positions
                 | ushift(chunk_sz, positions)
                 | dshift(chunk_sz, positions)
                 | lshift(chunk_sz, positions, chunk_mask)
                 | rshift(chunk_sz, positions, chunk_mask)).set(positions.size() - 1);
}

int main() {
    std::vector<std::string> lines;
    {
        std::string line;
        std::getline(std::cin, line);
        while (std::getline(std::cin.ignore(), line)) {
            if (line[2] == '#') {
                break;
            }
            line.resize(line.size() - 1);
            lines.push_back(line);
        }
    }
    auto chunk_sz = lines[0].size();
    auto sz = lines.size() * lines[0].size();
    // chunk mask is a bitset that has 1's at the start of each chunk
    auto positions = boost::dynamic_bitset(sz),
        chunk_mask = boost::dynamic_bitset(sz),
        u_blizz = boost::dynamic_bitset(sz),
        d_blizz = boost::dynamic_bitset(sz),
        l_blizz = boost::dynamic_bitset(sz),
        r_blizz = boost::dynamic_bitset(sz);
    for (auto i = std::size_t{0}; i < sz; i += chunk_sz) {
        chunk_mask.set(i);
    }
    for (auto i : ranges::views::iota(std::size_t{0}, lines.size())) {
        for (auto j : ranges::views::iota(std::size_t{0}, chunk_sz)) {
            switch (lines[i][j]) {
                case '^':
                    u_blizz[i * chunk_sz + j] = true;
                    break;
                case 'v':
                    d_blizz[i * chunk_sz + j] = true;
                    break;
                case '<':
                    l_blizz[i * chunk_sz + j] = true;
                    break;
                case '>':
                    r_blizz[i * chunk_sz + j] = true;
                    break;
            }
        }
    }
    auto step = 0;
    while (true) {
        ++step;
        optimistic_step_positions_fwd(positions, chunk_sz, chunk_mask);
        step_blizzards(u_blizz, d_blizz, l_blizz, r_blizz, chunk_sz, chunk_mask);
        positions &= ~(u_blizz | d_blizz | l_blizz | r_blizz);
        if (positions[positions.size() - 1]) {
            break;
        }
    }
    // step into the goal
    ++step;
    step_blizzards(u_blizz, d_blizz, l_blizz, r_blizz, chunk_sz, chunk_mask);
    positions.reset();
    while (true) {
        ++step;
        optimistic_step_positions_bkwd(positions, chunk_sz, chunk_mask);
        step_blizzards(u_blizz, d_blizz, l_blizz, r_blizz, chunk_sz, chunk_mask);
        positions &= ~(u_blizz | d_blizz | l_blizz | r_blizz);
        if (positions[0]) {
            break;
        }
    }
    // step into start
    ++step;
    step_blizzards(u_blizz, d_blizz, l_blizz, r_blizz, chunk_sz, chunk_mask);
    positions.reset();
    while (true) {
        ++step;
        optimistic_step_positions_fwd(positions, chunk_sz, chunk_mask);
        step_blizzards(u_blizz, d_blizz, l_blizz, r_blizz, chunk_sz, chunk_mask);
        positions &= ~(u_blizz | d_blizz | l_blizz | r_blizz);
        if (positions[positions.size() - 1]) {
            break;
        }
    }
    // step into the goal
    ++step;
//    step_blizzards(u_blizz, d_blizz, l_blizz, r_blizz, chunk_sz, chunk_mask);
//    positions.reset();
    std::cout << step << std::endl;
}
