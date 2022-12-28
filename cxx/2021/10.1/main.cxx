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

inline int scoreOf(char c) {
	switch (c) {
		case ')':
			return 3;
		case ']':
			return 57;
		case '}':
			return 1197;
		case '>':
			return 25137;
	}
	return 0;
}

int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);
	string s;
	int score{0};
	while (cin >> s) {
		vector<char> stack;
		for (char c : s) {
			switch (c) {
				case '{':
					stack.PB('}');
					continue;
				case '[':
					stack.PB(']');
					continue;
				case '(':
					stack.PB(')');
					continue;
				case '<':
					stack.PB('>');
					continue;
			}
			if (stack.empty() || stack.back() != c) {
				score += scoreOf(c);
				break;
			}
			stack.pop_back();
		}
	}
	cout << score << enl;
}
