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

int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);
	string initial;
	vector<vector<int>> init_field;
	{
		string r;
		while (cin >> r) {
			init_field.PB(vi{});
			for (auto c : r) {
				init_field.back().PB(c - '0');
			}
		}
	}
	vector<vector<int>> field(init_field.size() * 5);
	for (std::size_t ii{0}; ii < 5; ++ii) {
		for (std::size_t jj{0}; jj < 5; ++jj) {
			for (std::size_t i{0}; i < init_field.size(); ++i) {
				for (std::size_t j{0}; j < init_field[0].size(); ++j) {
					field[ii * init_field.size() + i].PB((init_field[i][j] + ii + jj - 1) % 9 + 1);
				}
			}
		}
	}
	priority_queue<tuple<int, std::size_t, std::size_t>, vector<tuple<int, std::size_t, std::size_t>>, greater<tuple<int, std::size_t, std::size_t>>> pq{};
	set<pss> visited{};
	pq.push({field.size() + field[0].size() - 2, 0, 0});
	while (!pq.empty()) {
		auto [score, x, y]{pq.top()};
		pq.pop();
		if (visited.count({x, y}) != 0) {
			continue;
		}
		visited.insert({x, y});
		if (x == field.size() - 1 && y == field[x].size() - 1) {
			cout << score << enl;
			return 0;
		}
		if (x + 1 < field.size()) {
			pq.push({score + field[x + 1][y] - 1, x + 1, y});
		}
		if (y + 1 < field[x].size()) {
			pq.push({score + field[x][y + 1] - 1, x, y + 1});
		}
		if (x > 0) {
			pq.push({score + field[x - 1][y] + 1, x - 1, y});
		}
		if (y > 0) {
			pq.push({score + field[x][y - 1] + 1, x, y - 1});
		}
	}
}
