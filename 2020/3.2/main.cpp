#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#define F first
#define S second
#define PB push_back
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
	vector<string> map;
	while (cin >> s) {
		map.PB(s);
	}
	std::size_t slen{map[0].size()}, x1{0}, x2{0}, x3{0}, x4{0}, x5{0};
	bool is_even{true};
	ll c1{0}, c2{0}, c3{0}, c4{0}, c5{0};
	for (const auto & r : map) {
		if (r[x1] == '#') {
			++c1;
		}
		if (r[x2] == '#') {
			++c2;
		}
		if (r[x3] == '#') {
			++c3;
		}
		if (r[x4] == '#') {
			++c4;
		}
		if (is_even && r[x5] == '#') {
			++c5;
		}
		x1 = (x1 + 1) % slen;
		x2 = (x2 + 3) % slen;
		x3 = (x3 + 5) % slen;
		x4 = (x4 + 7) % slen;
		if (is_even) {
			x5 = (x5 + 1) % slen;
		}
		is_even = !is_even;
	}
	cout << c1 * c2 * c3 * c4 * c5 << enl;
}
