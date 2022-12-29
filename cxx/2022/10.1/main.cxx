#include <bits/stdc++.h>

void check_cycle(auto &cycle, auto &xval, auto &strength_sum) {
    if ((cycle - 20) % 40 == 0) {
        strength_sum += xval * cycle;
    }
}

int main() {
    auto cycle = 0, xval = 1;
    auto strength_sum = 0ll;
    auto op = std::string{};
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
