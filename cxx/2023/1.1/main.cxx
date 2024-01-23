#include <bits/stdc++.h>

auto DIGITS = "0123456789";

int main() {
    auto line = std::string{};
    auto sum = 0;
    while (getline(std::cin, line)) {
        auto left = line.find_first_of(DIGITS);
        if (left == std::string::npos) {
            continue;
        }
        auto right = line.find_last_of(DIGITS);
        auto num = (line[left] - '0') * 10 + (line[right] - '0');
        sum += num;
    }
    std::cout << sum << std::endl;
}
