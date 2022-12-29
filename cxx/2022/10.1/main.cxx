#include <algorithm>
#include <ranges>
#include <vector>
#include <string>
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>

void check_cycle(auto &cycle, auto &xval, auto &strength_sum) {
    if ((cycle - 20) % 40 == 0) {
        strength_sum += xval * cycle;
    }
}

int main() {
    using ll = long long;
    int cycle = 0, xval = 1;
    ll strength_sum = 0;
    std::string op;
    while (std::cin >> op) {
        if (op == "noop") {
            ++cycle;
            check_cycle(cycle, xval, strength_sum);
        } else {
            ++cycle;
            check_cycle(cycle, xval, strength_sum);
            ++cycle;
            check_cycle(cycle, xval, strength_sum);
            int d;
            std::cin >> d;
            xval += d;
        }
    }
    std::cout << strength_sum << std::endl;
}
