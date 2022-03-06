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
	vector<vector<char>> grid;
	{
		char c;
		while (cin.peek() != std::char_traits<char>::eof()) {
			grid.PB(vector<char>{});
			while (cin.peek() != '\n' && cin.peek() != std::char_traits<char>::eof()) {
				cin >> c;
				grid.back().PB(c);
			}
			cin.get();
		}
	}
	const auto rows{grid.size()};
	const auto cols{grid[0].size()};
	int totalRiskLevel{0};
	for (std::size_t i{0}; i < rows; ++i) {
		for (std::size_t j{0}; j < cols; ++j) {
			bool top{i == 0}, bottom{i == rows - 1}, left{j == 0}, right{j == cols - 1};
			bool isMin{
				(top || grid[i - 1][j] > grid[i][j]) &&
				(bottom || grid[i + 1][j] > grid[i][j]) &&
				(left || grid[i][j - 1] > grid[i][j]) &&
				(right || grid[i][j + 1] > grid[i][j])
			};
			if (isMin) {
				totalRiskLevel += grid[i][j] - '0' + 1;
			}
		}
	}
	cout << totalRiskLevel << enl;
}
