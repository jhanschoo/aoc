#include <algorithm>
#include <ranges>
#include <vector>
#include <string>
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>

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
			if (itr - itl == 3) {
				break;
			}
		}
		std::cout << itr - line.begin() + 1 << std::endl;
	}
}
