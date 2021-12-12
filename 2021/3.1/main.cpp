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
	cin >> s;
	auto l{s.size()};
	vi onecounts(l, 0);
	int numstrs{0};
	do {
		++numstrs;
		for (std::size_t i{0}; i < l; ++i) {
			if (s[i] == '1') {
				++onecounts[i];
			}
		}
	} while (cin >> s);
	stringstream mostcommon;
	for (std::size_t i{0}; i < l; ++i) {
		mostcommon << (onecounts[i] > numstrs - onecounts[i] ? '1' : '0');
	}
	int gamma{0}, epsilon{0};
	char d;
	while (mostcommon >> d) {
		gamma <<= 1;
		epsilon <<= 1;
		if (d == '1') {
			++gamma;
		} else {
			++epsilon;
		}
	}
	cout << gamma * epsilon << enl;
}
