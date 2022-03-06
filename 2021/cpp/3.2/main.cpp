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
	string s;
	vector<string> o2cands;
	while (cin >> s) {
		o2cands.PB(s);
	}
	vector<string> co2cands{o2cands};
	auto l{o2cands[0].size()};
	for (std::size_t i{0}; i < l && o2cands.size() > 1; ++i) {
		int delta{0};
		for (CAN s : o2cands) {
			if (s[i] == '1') {
				delta++;
			} else {
				delta--;
			}
		}
		char necessarybit{delta >= 0 ? '1' : '0'};
		auto res = remove_if(o2cands.begin(), o2cands.end(), [&](CAN s) {
			return s[i] != necessarybit;
		});
		o2cands.erase(res, o2cands.end());
	}
	cout << enl << enl;
	for (std::size_t i{0}; i < l && co2cands.size() > 1; ++i) {
		int delta{0};
		for (CAN s : co2cands) {
			if (s[i] == '1') {
				delta++;
			} else {
				delta--;
			}
		}
		char necessarybit{delta >= 0 ? '0' : '1'};
		auto res = remove_if(co2cands.begin(), co2cands.end(), [&](CAN s) {
			return s[i] != necessarybit;
		});
		co2cands.erase(res, co2cands.end());
	}
	int o2{0}, co2{0};
	for (CAN c : o2cands[0]) {
		o2 <<= 1;
		if (c == '1') {
			++o2;
		}
	}
	for (CAN c : co2cands[0]) {
		co2 <<= 1;
		if (c == '1') {
			++co2;
		}
	}
	cout << o2cands[0] << " " << co2cands[0] << enl;
	cout << o2 << " " << co2 << " " << o2 * co2 << enl;
}
