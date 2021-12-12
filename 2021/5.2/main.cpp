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

int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);
	array<array<int, 1000>, 1000> field{{0}};
	std::size_t x1, y1, x2, y2;
	char c1, c2;
	string arrow;
	while (cin >> x1 >> c1 >> y1 >> arrow >> x2 >> c2 >> y2) {
		if (x1 > x2) {
			swap(x1, x2);
			swap(y1, y2);
		}
		if (x1 == x2) {
			if (y1 > y2) {
				swap(y1, y2);
			}
			for (; y1 <= y2; ++y1) {
				++field[x1][y1];
			}
		} else if (y1 == y2) {
			for (; x1 <= x2; ++x1) {
				++field[x1][y1];
			}
		} else if (y1 < y2) {
			for (; x1 <= x2; ++x1, ++y1) {
				++field[x1][y1];
			}
		} else if (y1 > y2) {
			for (; x1 <= x2; ++x1, --y1) {
				++field[x1][y1];
			}
		} else {
			cout << "Impossible (" << x1 << "," << x2 << ") -> (" << y1 << "," << y2 << ")" << enl;
		}
	}
	int overlaps{0};
	for (std::size_t i{0}; i < 1000; ++i) {
		for (std::size_t j{0}; j < 1000; ++j) {
			// cout << field[i][j];
			if (field[i][j] > 1) {
				++overlaps;
			}
		}
		// cout << enl;
	}
	cout << overlaps << enl;
}
