#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#define F first
#define S second
#define PB push_back
#define CAN const auto &
#define REP(i,a,b) for(int i{a}; i <= b; ++i)
#define enl "\n"
#define dbg(x) cout << "(dbg) " << x << enl

using namespace __gnu_pbds;
using namespace std;

using indexed_set = tree<int, null_type, less<int>, rb_tree_tag, tree_order_statistics_node_update>;
using ll = long long;
using vi = vector<int>;
using vl = vector<ll>;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using pss = pair<std::size_t, std::size_t>;

void step(int &x, int &y, int &dx, int &dy) {
	x += dx;
	y += dy;
	if (dx > 0) {
		--dx;
	} else if (dx < 0) {
		++dx;
	}
	--dy;
}

tuple<int, int, int> simulate(int dx, int dy, int x_start, int y_start, int x_beyond, int y_beyond) {
	int x{0}, y{0}, highest{0};
	while (x <= x_beyond && y >= y_beyond) {
		step(x, y, dx, dy);
		highest = max(highest, y);
		if (x >= x_start && y <= y_start) {
			return {x, y, highest};
		}
	}
	return {x, y, highest};
}

int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);
	char c;
	int x_start, y_start, x_beyond, y_beyond;
	while ((cin >> c) && c != '=') { }
	cin >> x_start;
	cin.get(); cin.get();
	cin >> x_beyond;
	if (x_start > x_beyond) {
		swap(x_start, x_beyond);
	}
	while ((cin >> c) && c != '=') { }
	cin >> y_start;
	cin.get(); cin.get();
	cin >> y_beyond;
	if (y_start < y_beyond) {
		swap(y_start, y_beyond);
	}
	int found{0};
	for (int dy{-y_beyond}; dy >= y_beyond; --dy) {
		for (int dx{0}; dx <= x_beyond; ++dx) {
			auto [x, y, highest] = simulate(dx, dy, x_start, y_start, x_beyond, y_beyond);
			if (x >= x_start && x <= x_beyond && y <= y_start && y >= y_beyond) {
				++found;
			}
		}
	}
	cout << found << enl;
}
