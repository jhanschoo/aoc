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
	vector<string> sv;
	{
		string s;
		while (getline(cin, s)) {
			sv.PB(s);
		}
	}
	auto max_magnitude{0LL};
	for (auto itl{sv.begin()}; itl != sv.end(); ++itl) {
		for (auto itr{sv.begin()}; itr != sv.end(); ++itr) {
			if (itl == itr) {
				continue;
			}
			string l{*itl}, r{*itr};
			istringstream l_iss{l}, r_iss{r};
			auto m{(snailfish{l_iss} + snailfish{r_iss}).get_magnitude()};
			if (m > max_magnitude) {
				max_magnitude = m;
			}
		}
	}
	cout << max_magnitude << enl;
}
