#include <bits/stdc++.h>

int main() {
	int il, ih, jl, jh;
	char h1, cc, h2;
	auto overlaps = 0;
	while (std::cin >> il >> h1 >> ih >> cc >> jl >> h2 >> jh) {
		if (jl < il) {
			std::swap(il, jl);
			std::swap(ih, jh);
		}
		if (jl <= ih) {
			overlaps++;
		}
	}
	std::cout << overlaps << std::endl;
}
