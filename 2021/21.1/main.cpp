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

constexpr inline int next(int prev_moves) {
	return (prev_moves + 8) % 100 + 1;
}

int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);
	int p1pos, p2pos, p1score{0}, p2score{0}, moves{6};
	ll rolls{0};
	char c;
	while ((cin >> c) && c != ':') {}
	cin.get();
	cin >> p1pos;
	while ((cin >> c) && c != ':') {}
	cin.get();
	cin >> p2pos;
	bool p1turn{true};
	while (p1score < 1000 && p2score < 1000) {
		int &pos{p1turn ? p1pos : p2pos};
		int &score{p1turn ? p1score : p2score};
		pos = (pos + moves - 1) % 10 + 1;
		score += pos;
		p1turn = !p1turn;
		moves = next(moves);
		++rolls;
	}
	cout << p1score << " " << p2score << " " << rolls * 3 << " " << min(p1score, p2score) * rolls * 3 << enl;
}
