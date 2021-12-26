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
using szt = std::size_t;
using pss = pair<szt, szt>;

int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);
	vector<string> state, prev_state, intermediate_state;
	string s;
	while (cin >> s) {
		state.PB(s);
	}
	int steps{0};
	while (state != prev_state) {
		prev_state = state;
		for (szt i{0}; i < state.size(); ++i) {
			for (szt j{0}; j < state[i].size(); ++j) {
				auto j_right{(j + 1) % state[i].size()};
				if (prev_state[i][j] == '>' && prev_state[i][j_right] == '.') {
					state[i][j] = '.';
					state[i][j_right] = '>';
				}
			}
		}
		intermediate_state = state;
		for (szt i{0}; i < state.size(); ++i) {
			auto i_down{(i + 1) % state.size()};
			for (szt j{0}; j < state[i].size(); ++j) {
				if (intermediate_state[i][j] == 'v' && intermediate_state[i_down][j] == '.') {
					state[i][j] = '.';
					state[i_down][j] = 'v';
				}
			}
		}
		// for (const auto &s : state) {
		// 	cout << s << enl;
		// }
		// cout << enl;
		++steps;
	}
	cout << steps << enl;
	return 0;
}
