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
	vi pos;
	{
		std::size_t t;
		char c;
		while (cin >> t) {
			pos.PB(t);
			cin >> c;
		}
	}
	sort(pos.begin(), pos.end());
	int median = pos[pos.size() / 2];
	ll fuel{0};
	for (CAN i : pos) {
		fuel += abs(i - median);
	}
	cout << fuel << enl;
}
