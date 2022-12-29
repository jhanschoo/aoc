#include <algorithm>
#include <ranges>
#include <vector>
#include <string>
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>

auto snafu_to_decimal(std::string_view snafu) {
    long long decimal = 0;
    for (auto &&c: snafu) {
        decimal *= 5;
        switch (c) {
            case '2':
                decimal += 2;
                break;
            case '1':
                decimal += 1;
                break;
            case '0':
                break;
            case '-':
                decimal -= 1;
                break;
            case '=':
                decimal -= 2;
                break;
        }
    }
    return decimal;
}

auto decimal_to_snafu(long long i) {
    std::vector<char> snafu;
    while (i != 0) {
        auto rem = (i + 2) % 5;
        switch (rem) {
            case 4:
                snafu.push_back('2');
                i -= 2;
                break;
            case 3:
                snafu.push_back('1');
                i -= 1;
                break;
            case 2:
                snafu.push_back('0');
                break;
            case 1:
                snafu.push_back('-');
                i += 1;
                break;
            case 0:
                snafu.push_back('=');
                i += 2;
                break;
        }
        i /= 5;
    }
    return std::string{snafu.rbegin(), snafu.rend()};
}

int main() {
    std::string line;
    long long sum = 0;
    while (std::cin >> line) {
        sum += snafu_to_decimal(line);
        std::cout << sum << " " << decimal_to_snafu(sum) << std::endl;
    }
}
