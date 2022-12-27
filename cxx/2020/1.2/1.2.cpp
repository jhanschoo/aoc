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
	for (std::size_t i{0}; i < nums.size() - 2; ++i) {
		std::size_t j{i + 1}, k{nums.size() - 1};
		while (j < k) {
			if (nums[i] + nums[j] + nums[k] > 2020) {
				--k;
			} else if (nums[i] + nums[j] + nums[k] < 2020) {
				++j;
			} else {
				cout << nums[i] * nums[j] * nums[k] << enl;
				return 0;
			}
		}
	}
}
