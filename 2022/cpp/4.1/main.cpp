#include <algorithm>
#include <ranges>
#include <vector>
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>

int main() {
	int il, ih, jl, jh;
	char h1, cc, h2;
	int full_contains = 0;
	while (std::cin >> il >> h1 >> ih >> cc >> jl >> h2 >> jh) {
		if ((il <= jl && ih >= jh) || (il >= jl && ih <= jh)) {
			full_contains++;
		}
	}
	std::cout << full_contains << std::endl;
}
