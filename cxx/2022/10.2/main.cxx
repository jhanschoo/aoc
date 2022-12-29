#include <bits/stdc++.h>

void check_cycle(auto &cycle, auto &xval, auto &crt) {
    auto pixel = cycle % 40;
    if (pixel == 0) {
        crt.push_back(std::vector<bool>(40, false));
    }
    if (pixel >= xval - 1 && pixel <= xval + 1) {
        crt.back()[pixel] = true;
    }
}

int main() {
    auto cycle = 0, xval = 1;
    auto crt = std::vector<std::vector<bool>>{};
    auto op = std::string{};
    while (std::cin >> op) {
        if (op == "noop") {
            check_cycle(cycle, xval, crt);
            ++cycle;
        } else {
            check_cycle(cycle, xval, crt);
            ++cycle;
            check_cycle(cycle, xval, crt);
            ++cycle;
            int d;
            std::cin >> d;
            xval += d;
        }
    }
    for (auto &&row: crt) {
        for (auto &&pixel: row) {
            std::cout << (pixel ? '#' : '.');
        }
        std::cout << std::endl;
    }
}
