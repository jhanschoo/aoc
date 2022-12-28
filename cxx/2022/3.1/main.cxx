#include <bits/stdc++.h>
#include <range/v3/all.hpp>

constexpr auto char_to_priority(char c) {
	if (c <= 'Z') {
		return c - 'A' + 27ll;
	}
	return c - 'a' + 1ll;
}

int main() {
    auto sum = 0ll;
    auto rucksack = std::string{};
    while (std::cin >> rucksack) {
        auto median = rucksack.size() / 2;
        auto chunks = rucksack | ranges::view::chunk(median);
        ranges::for_each(chunks, ranges::sort);
        auto intersection = std::vector<char>{};
        ranges::set_intersection(chunks[0], chunks[1], std::back_inserter(intersection));

        sum += char_to_priority(intersection[0]);
    }
	std::cout << sum << std::endl;
	return 0;
}
