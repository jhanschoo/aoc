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

inline void process(const pss &p, vector<vector<int>> &field, vector<pss> &to_flash) {
	const auto &[i, j]{p};
	++field[i][j];
	if (field[i][j] > 9) {
		field[i][j] = -10;
		to_flash.PB(p);
	}
}

int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);
	vector<vector<int>> field;
	{
		char ch;
		vector<int> row;
		while ((ch = cin.get()) != std::char_traits<char>::eof()) {
			if (ch == '\n') {
				field.push_back(row);
				row.clear();
			} else {
				row.push_back(ch - '0');
			}
		}
		if (!row.empty()) {
			field.push_back(row);
		}
	}
	std::size_t n{field.size()}, m{field[0].size()};
	int total_flashes{0};
	REP(iteration, 1, 100) {
		int flashes_this_round{0};
		vector<pss> to_flash, to_zero;
		for (std::size_t i{0}; i < n; ++i) {
			for (std::size_t j{0}; j < m; ++j) {
				process({i, j}, field, to_flash);
			}
		}
		while (!to_flash.empty()) {
			total_flashes += to_flash.size();
			flashes_this_round += to_flash.size();
			vector<pss> to_flash_next;
			for (const auto &p : to_flash) {
				auto [i, j]{p};
				field[i][j] = -10;
				if (i > 0 && j > 0) {
					process({i - 1, j - 1}, field, to_flash_next);
				}
				if (i > 0) {
					process({i - 1, j}, field, to_flash_next);
				}
				if (i > 0 && j < m - 1) {
					process({i - 1, j + 1}, field, to_flash_next);
				}
				if (j > 0) {
					process({i, j - 1}, field, to_flash_next);
				}
				if (j < m - 1) {
					process({i, j + 1}, field, to_flash_next);
				}
				if (i < n - 1 && j > 0) {
					process({i + 1, j - 1}, field, to_flash_next);
				}
				if (i < n - 1) {
					process({i + 1, j}, field, to_flash_next);
				}
				if (i < n - 1 && j < m - 1) {
					process({i + 1, j + 1}, field, to_flash_next);
				}
			}
			to_zero.insert(to_zero.end(), to_flash.begin(), to_flash.end());
			to_flash = to_flash_next;
		}
		for (const auto &p : to_zero) {
			auto [i, j]{p};
			field[i][j] = 0;
		}
	}
	cout << "Total flashes: " << total_flashes << enl;
}
