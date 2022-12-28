#include <bits/stdc++.h>

auto compute_score(auto l, auto r) {
    l -= 'A';
    r -= 'X';
    auto result = (r + 4 - l) % 3;
    return r + 1 + 3 * result;
}

int main() {
	std::ios::sync_with_stdio(0);
	std::cin.tie(0);
    auto l = char{}, r = char{};
	auto score = 0ll;
	while (std::cin >> l >> r) {
		score += compute_score(l, r);
	}
	std::cout << score << std::endl;
}
