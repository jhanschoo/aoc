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

int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);
	string initial;
	map<pair<char, char>, char> rules;
	cin >> initial;
	{
		char c1, c2, r;
		string arrow;
		while (cin >> c1 >> c2 >> arrow >> r) {
			rules[{c1, c2}] = r;
		}
	}
	map<pair<char, char>, std::size_t> pairs;
	for (auto it{initial.begin() + 1}; it < initial.end(); ++it) {
		++pairs[{*(it - 1), *it}];
	}
	map<char, std::size_t> singles;
	for (auto &c : initial) {
		++singles[c];
	}
	REP(iterations, 1, 10) {
		map<pair<char, char>, std::size_t> new_pairs;
		for (auto &entry : pairs) {
			auto &[p, n]{entry};
			auto &[c1, c2]{p};
			char r{rules[p]};
			new_pairs[{c1, r}] += n;
			new_pairs[{r, c2}] += n;
			singles[r] += n;
		}
		pairs = new_pairs;
	}
	auto maxi{singles.begin()->S}, mini{singles.begin()->S};
	for (auto &entry : singles) {
		auto &[c, n]{entry};
		if (n > maxi) {
			maxi = n;
		}
		if (n < mini) {
			mini = n;
		}
	}
	cout << maxi - mini << enl;
}
