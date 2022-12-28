#include <bits/stdc++.h>

auto compute_score(auto l, auto result) {
    l -= 'A';
    result -= 'Y';
    auto r = (l + result + 3) % 3;
    result += 1;
    return r + 1 + 3 * result;
}

int main() {
	std::ios::sync_with_stdio(0);
	std::cin.tie(0);
    auto l = char{}, result = char{};
	auto score = 0ll;
	while (std::cin >> l >> result) {
		score += compute_score(l, result);
	}
	std::cout << score << std::endl;
}
