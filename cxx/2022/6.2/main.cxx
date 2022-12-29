#include <bits/stdc++.h>

int main() {
    std::string line;
    while (std::getline(std::cin, line)) {
        auto itl = line.begin();
        auto itr = itl;
        while (++itr < line.end()) {
            for (auto it = itl; it != itr; ++it) {
                if (*it == *itr) {
                    itl = it + 1;
                    break;
                }
            }
            if (itr - itl == 13) {
                break;
            }
        }
        std::cout << itr - line.begin() + 1 << std::endl;
    }
}
