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
	array<int, 7> mature{0}, youth{0};
	{
		std::size_t t;
		char c;
		while (cin >> t) {
			++mature[t];
			cin >> c;
		}
	}
	int total_days{80};
	for (int day{0}; day < total_days; ++day) {
		std::size_t i{static_cast<std::size_t>(day % 7)};
		youth[(i + 2) % 7] = mature[i];
		mature[i] += youth[i];
		youth[i] = 0;
	}
	int total{0};
	for (CAN i : mature) { total += i; }
	for (CAN i : youth) { total += i; }
	cout << total << enl;
}
