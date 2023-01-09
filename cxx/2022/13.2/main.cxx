#include <bits/stdc++.h>
#include <range/v3/all.hpp>

bool is_significant_character(int c) {
    return ('0' <= c && c <= '9') || c == '[' || c == ']';
}

bool in_correct_order(const std::string &left, const std::string &right) {
    auto lss = std::istringstream{left}, rss = std::istringstream{right};
    // inserted terminators after immediate delta: number of additional inserted terminators after the immediate number
    auto itaid = int{};
    while (true) {
        // invariant(itaid) holds: if itaid is nonzero, one of the streams is not terminal and has a number
        while (lss && !is_significant_character(lss.peek())) { lss.get(); }
        while (rss && !is_significant_character(rss.peek())) { rss.get(); }
        if (!lss) { return true; }
        if (!rss) { /* We have lss and !rss. */ return false; }
        if (lss.peek() == '[' && rss.peek() == '[') { lss.get(); rss.get(); continue; }
        if (lss.peek() == '[' && rss.peek() != ']') { lss.get(); ++itaid; continue; }
        if (rss.peek() == '[' && lss.peek() != ']') { rss.get(); --itaid; continue; }
        if (lss.peek() == ']') {
            if (rss.peek() == ']') {
                lss.get(); rss.get();
                continue;
            }
            // otherwise rss is an integer or '['
            return true;
        }
        if (rss.peek() == ']') { /* lss is an integer or '[' */ return false; }

        // invariant: lss and rss are both integers
        int ld, rd;
        lss >> ld; rss >> rd;
        if (ld < rd) { return true; }
        if (ld > rd) { return false; }
        // resolve itaids
        while (itaid != 0) {
            while (lss && !is_significant_character(lss.peek())) { lss.get(); }
            while (rss && !is_significant_character(rss.peek())) { rss.get(); }
            if (!lss) { return true; }
            if (!rss) { /* We have lss and !rss. */ return false; }
            if (lss.peek() == ']' && itaid > 0) { lss.get(); --itaid; continue; }
            if (rss.peek() == ']' && itaid < 0) { rss.get(); ++itaid; continue; }
            if (itaid > 0) {
                // rss is ']' but lss has more at current level
                return false;
            }
            // lss is ']' but rss has more at current level
            return true;
        }
    }
}

int main() {
    auto div1 = std::string{"[[2]]"}, div2 = std::string{"[[6]]"};
    auto packets = std::vector<std::string>{div1, div2};
    auto buffer = std::string{};
    while (std::cin >> buffer) { packets.push_back(buffer); }
    ranges::sort(packets, in_correct_order);
    auto div1_index = ranges::distance(packets.begin(), ranges::find(packets, div1)) + 1;
    auto div2_index = ranges::distance(packets.begin(), ranges::find(packets, div2)) + 1;
    std::cout << div1_index * div2_index << std::endl;
}
