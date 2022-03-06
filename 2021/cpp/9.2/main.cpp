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
	vector<pair<std::size_t, std::size_t>> sinks;
	vi sinkSizes;
	const auto rows{grid.size()};
	const auto cols{grid[0].size()};
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
				sinks.PB({i, j});
				sinkSizes.PB(0);
			}
		}
	}
	for (std::size_t t{0}; t < sinks.size(); ++t) {
		auto [i, j] = sinks[t];
		auto &size{sinkSizes[t]};
		vector<tuple<std::size_t, std::size_t>> stack;
		stack.PB({i, j});
		while (!stack.empty()) {
			auto [i, j] = stack.back();
			stack.pop_back();
			if (grid[i][j] == '9') {
				continue;
			}
			if (i > 0 && grid[i - 1][j] != '9' && grid[i-1][j] > grid[i][j]) {
				stack.PB({i - 1, j});
			}
			if (i < rows - 1 && grid[i + 1][j] != '9' && grid[i+1][j] > grid[i][j]) {
				stack.PB({i + 1, j});
			}
			if (j > 0 && grid[i][j - 1] != '9' && grid[i][j-1] > grid[i][j]) {
				stack.PB({i, j - 1});
			}
			if (j < cols - 1 && grid[i][j + 1] != '9' && grid[i][j+1] > grid[i][j]) {
				stack.PB({i, j + 1});
			}
			grid[i][j] = '9';
			++size;
		}
	}
	int first{0}, second{0}, third{0};
	for (CAN size : sinkSizes) {
		if (size >= first) {
			third = second;
			second = first;
			first = size;
		} else if (size >= second) {
			third = second;
			second = size;
		} else if (size >= third) {
			third = size;
		}
	}
	cout << first * second * third << enl;
}
