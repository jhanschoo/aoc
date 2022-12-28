#include <bits/stdc++.h>

constexpr auto char_to_priority(char c) {
	if (c <= 'Z') {
		return c - 'A' + 27;
	}
	return c - 'a' + 1;
}

int main() {
    auto s1 = std::string{}, s2 = std::string{}, s3 = std::string{};
	auto sum = 0;
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
