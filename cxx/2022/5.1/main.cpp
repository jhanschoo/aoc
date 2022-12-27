#include <algorithm>
#include <ranges>
#include <vector>
#include <string>
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>

constexpr size_t num_stacks = 9;

int main() {
	std::string line;
	std::array<std::vector<char>, num_stacks> stacks;
	while (std::getline(std::cin, line)) {
		if (line.size() >= 2 && line[1] == '1') {
			break;
		}
		for (size_t i = 0; i * 4 + 1 < line.size(); ++i) {
			auto c = line[i * 4 + 1];
			if (c != ' ') {
				stacks[i].push_back(c);
			}
		}
	}
	for (auto &&stack : stacks) {
		std::ranges::reverse(stack);
	}
	std::string s1, s2, s3;
	size_t count, from, to;
	while (std::cin >> s1 >> count >> s2 >> from >> s3 >> to) {
		--from; --to;
		for (size_t i = 0; i < count; ++i) {
			stacks[to].push_back(stacks[from].back());
			stacks[from].pop_back();
		}
	}
	for (auto &&stack : stacks) {
		if (stack.empty()) {
			std::cout << ' ';
		} else {
			std::cout << stack.back();
		}
	}
	std::cout << std::endl;
}
