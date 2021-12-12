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
	int low, high, count{0};
	char dash, c;
	string skip, pass;
	while (cin >> low >> dash >> high >> c >> skip >> pass) {
		if (pass[low - 1] == c) {
			if (pass[high - 1] != c) {
				count++;
			}
			continue;
		}
		if (pass[high - 1] == c) {
			count++;
		}
	}
	cout << count << enl;
}
