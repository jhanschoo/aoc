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

inline void process_fold(set<pii> &by_x, set<pii> &by_y, char axis, int val) {
	if (axis == 'x') {
		vector<pii> to_remove{by_x.lower_bound({val, 0}), by_x.end()};
		for (auto &p : to_remove) {
			auto &[x, y] = p;
			auto new_x{val - (x - val)};
			by_x.erase(p);
			by_y.erase({y, x});
			by_x.insert({new_x, y});
			by_y.insert({y, new_x});
		}
	} else {
		vector<pii> to_remove{by_y.lower_bound({val, 0}), by_y.end()};
		for (auto &p : to_remove) {
			auto &[y, x] = p;
			auto new_y{val - (y - val)};
			by_y.erase(p);
			by_x.erase({x, y});
			by_y.insert({new_y, x});
			by_x.insert({x, new_y});
		}
	}
}

int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);
	set<pii> by_x, by_y;
	while (cin.peek() != '\n') {
		int x, y;
		char c;
		cin >> x >> c >> y;
		by_x.insert({x, y});
		by_y.insert({y, x});
		cin.get();
	}
	cin.get();
	while (cin.peek() != std::char_traits<char>::eof()) {
		string s1, s2;
		char axis, c;
		int val;
		cin >> s1 >> s2 >> axis >> c >> val;
		process_fold(by_x, by_y, axis, val);
		cin.get();
		cout << "After folding along " << axis << " = " << val << ", there are " << by_x.size() << " points." << enl;
	}
}
