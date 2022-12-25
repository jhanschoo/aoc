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
	// using namespace __gnu_pbds;
	// using namespace std;
	// using indexed_set = tree<int, null_type, less<int>, rb_tree_tag, tree_order_statistics_node_update>;
	// using ll = long long;
	// using vi = vector<int>;
	// using vl = vector<ll>;
	// using pii = pair<int, int>;
	// using pll = pair<ll, ll>;
	int sum = 0;
	std::ranges::for_each(std::ranges::istream_view<std::string>(std::cin), [&sum](auto&& rucksack) {
		auto median = rucksack.size() / 2;
		auto left = rucksack | std::ranges::views::take(median);
		auto right = rucksack | std::ranges::views::drop(median);
		std::ranges::sort(left);
		std::ranges::sort(right);
		auto intersection = std::vector<char>{};
		std::ranges::set_intersection(left, right, std::back_inserter(intersection));
		sum += char_to_priority(intersection[0]);
	});
	std::cout << sum << std::endl;
	return 0;
}
