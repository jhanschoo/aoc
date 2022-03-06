#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include "snailfish.h"
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

int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);
	string s;
	getline(cin, s);
	auto iss{istringstream{s}};
	auto sf{snailfish{iss}};
	while (getline(cin, s)) {
		auto iss1{istringstream{s}};
		auto sf1{snailfish{iss1}};
		sf = sf + sf1;
	}
	cout << sf << enl;
	cout << sf.get_magnitude() << enl;
	return 0;
}
