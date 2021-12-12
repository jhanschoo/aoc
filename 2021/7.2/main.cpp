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

int fuelFor(const vi &pos, int dest) {
	int ret{0};
	for (CAN p : pos) {
		int delta{abs(p - dest)};
		ret += (delta * (delta + 1)) / 2;
	}
	return ret;
}

int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);
	vi pos;
	std::size_t total{0};
	{
		std::size_t t;
		char c;
		while (cin >> t) {
			pos.PB(t);
			total += t;
			cin >> c;
		}
	}
	std::size_t avg{total / pos.size()};
	cout << min(fuelFor(pos, avg), fuelFor(pos, avg + 1)) << enl;
}
