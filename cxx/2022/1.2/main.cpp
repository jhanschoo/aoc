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
	vector<int> maximums = {0, 0, 0};
	int current{0};
	string s;
	while (getline(cin, s)) {
		if (s.size()) {
			auto n{stoi(s)};
			current += n;
		} else {
			int smallest{current};
			std::size_t smallest_index{3};
			for (std::size_t i{0}; i < maximums.size(); ++i) {
				if (maximums[i] < smallest) {
					smallest = maximums[i];
					smallest_index = i;
				}
			}
      if (smallest_index != 3) {
        maximums[smallest_index] = current;
      }
			current = 0;
		}
	}
  auto sum{0};
  for (auto n : maximums) {
    sum += n;
  }
	cout << sum << enl;
}
