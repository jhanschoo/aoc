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
	constexpr array<bool, 8> isUnique{false, false, true, true, true, false, false, true};
	ios::sync_with_stdio(0);
	cin.tie(0);
	int count{0};
	{
		while (cin.peek() != std::char_traits<char>::eof()) {
			string s;
			while (cin >> s && s != "|") {
			}
			while (cin >> s) {
				if (isUnique[s.size()]) {
					++count;
				}
				if (cin.get() == '\n') {
					break;
				}
			}
		}
	}
	cout << count << enl;
}
