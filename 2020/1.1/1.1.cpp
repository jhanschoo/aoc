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
	vi nums;
	int buf;
	while (cin >> buf) {
		nums.PB(buf);
	}
	sort(nums.begin(), nums.end());
	auto r{nums.size() - 1}, l{nums.size() - nums.size()};
	while (l < r) {
		if (nums[l] + nums[r] == 2020) {
			cout << nums[l] * nums[r] << enl;
			return 0;
		} else if (nums[l] + nums[r] < 2020) {
			++l;
		} else {
			--r;
		}
	}
}
