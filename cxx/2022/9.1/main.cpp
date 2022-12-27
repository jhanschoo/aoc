#include <algorithm>
#include <ranges>
#include <vector>
#include <string>
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>

void move_tail(auto& tail, auto& head, auto& seen) {
	auto& [x, y] = tail;
	auto& [hx, hy] = head;
	// distance eastward the head is from the tail
	auto dx = hx - x;
	// distance northward the head is from the tail
	auto dy = hy - y;
	if (dx == 0) {
		// if same longitude
		if (dy > 1) {
			// head north of tail
			++y;
		} else if (dy < -1) {
			// head south of tail
			--y;
		}
	} else if (dy == 0) {
		// if same latitude
		if (dx > 1) {
			// head east of tail
			++x;
		} else if (dx < -1) {
			// head west of tail
			--x;
		}
	} else if (dx > 0 && dy > 0 && dx + dy > 2) {
		// head northeast of tail
		++x; ++y;
	} else if (dx > 0 && dy < 0 && dx - dy > 2) {
		// head northwest of tail
		++x; --y;
	} else if (dx < 0 && dy > 0 && dy - dx > 2) {
		// head southeast of tail
		--x; ++y;
	} else if (dx < 0 && dy < 0 && dx + dy < -2) {
		// head southwest of tail
		--x; --y;
	}
	seen.insert(tail);
}

int main() {
	using ll = long long;
	auto seen = std::set<std::pair<ll, ll>> {{0, 0}};
	auto head = std::pair<ll, ll> {0, 0}, tail = std::pair<ll, ll> {0, 0};
	char direction;
	int steps;
	while (std::cin >> direction >> steps) {
		for (int s = 0; s < steps; ++s) {
			switch (direction) {
				case 'U': ++head.second; break;
				case 'D': --head.second; break;
				case 'R': ++head.first; break;
				case 'L': --head.first; break;
			}
			move_tail(tail, head, seen);
		}
	}
	std::cout << seen.size() << std::endl;
}
