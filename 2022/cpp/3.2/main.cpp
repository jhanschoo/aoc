#include <algorithm>
#include <ranges>
#include <vector>
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>

constexpr auto char_to_priority(char c) {
	if (c <= 'Z') {
		return c - 'A' + 27;
	}
	return c - 'a' + 1;
}

int main() {
	std::string s1, s2, s3;
	int sum = 0;
	while (std::cin >> s1 >> s2 >> s3) {
		std::ranges::sort(s1);
		std::ranges::sort(s2);
		std::ranges::sort(s3);
		std::string inter{}, final{};
		std::ranges::set_intersection(s1, s2, std::back_inserter(inter));
		std::ranges::set_intersection(inter, s3, std::back_inserter(final));
		sum += char_to_priority(final[0]);
	}
	std::cout << sum << std::endl;
}
