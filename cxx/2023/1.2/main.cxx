#include <bits/stdc++.h>
#include <range/v3/all.hpp>

std::array<std::string, 10> DIGIT_WORDS = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
std::array<char, 10> DIGITS = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

int main() {
    auto line = std::string{};
    std::size_t sum = 0;
    while (getline(std::cin, line)) {
        std::size_t l_idx = std::string::npos, r_idx = 0;
        std::size_t l_num = 0, r_num = 0;
        for (const auto & [digit, digit_repr] : ranges::views::enumerate(DIGIT_WORDS)) {
            auto l_idx_curr = line.find(digit_repr);
            if (l_idx_curr != std::string::npos && l_idx_curr < l_idx) {
                l_idx = l_idx_curr;
                l_num = digit;
            }
            auto r_idx_curr = line.rfind(digit_repr);
            if (r_idx_curr != std::string::npos && r_idx_curr >= r_idx) {
                r_idx = r_idx_curr;
                r_num = digit;
            }
        }
        // copy-paste of above, but different types
        for (const auto & [digit, digit_repr] : ranges::views::enumerate(DIGITS)) {
            auto l_idx_curr = line.find(digit_repr);
            if (l_idx_curr != std::string::npos && l_idx_curr < l_idx) {
                l_idx = l_idx_curr;
                l_num = digit;
            }
            auto r_idx_curr = line.rfind(digit_repr);
            if (r_idx_curr != std::string::npos && r_idx_curr >= r_idx) {
                r_idx = r_idx_curr;
                r_num = digit;
            }
        }

        auto num = l_num * 10 + r_num;
        sum += num;
    }
    std::cout << sum << std::endl;
}
