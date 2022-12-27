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

using point = pair<std::size_t, std::size_t>;
using pointmap = unordered_map<int, point>;
using cell = pair<int, bool>;
using row_t = vector<cell>;
using board_t = vector<row_t>;
using boards_t = vector<tuple<board_t, pointmap, bool>>;

bool createsWin(const board_t &board, const point &lastPoint) {
	// check vertical
	bool has_win{true};
	for (std::size_t y{0}; y < board.size(); ++y) {
		if (!board[y][lastPoint.second].second) {
			has_win = false;
			break;
		}
	}
	if (has_win) {
		return true;
	}
	// check horizontal
	has_win = true;
	for (std::size_t x{0}; x < board.size() /* valid since board is square */; ++x) {
		if (!board[lastPoint.first][x].second) {
			has_win = false;
			break;
		}
	}
	return has_win;
}

int score(const board_t board) {
	int score{0};
	for (std::size_t y{0}; y < board.size(); ++y) {
		for (std::size_t x{0}; x < board.size(); ++x) {
			if (!board[y][x].second) {
				score += board[y][x].first;
			}
		}
	}
	return score;
}

int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);
	vi ns;
	{
		int tmpi;
		do {
			cin >> tmpi;
			ns.PB(tmpi);
		} while (cin.get() == ',');
	}
	cin.get();
	boards_t boards;
	for (; cin.good(); cin.get()) {
		board_t board;
		pointmap coords;
		for (std::size_t y{0}; cin.peek() != '\n' && cin.good(); ++y, cin.get()) {
			row_t row;
			for (std::size_t x{0}; cin.peek() != '\n' && cin.good(); ++x) {
				int i;
				cin >> i;
				row.PB({i, false});
				coords[i] = {y, x};
			}
			board.PB(row);
		}
		boards.PB({board, coords, false});
	}
	auto unwonBoards{boards.size()};
	for (CAN n : ns) {
		for (auto &[board, coords, hasWon] : boards) {
			if (hasWon) {
				continue;
			}
			const auto lastPointPair{coords.find(n)};
			if (lastPointPair == coords.end()) {
				continue;
			}
			const auto &lastPoint{lastPointPair->S};
			board[lastPoint.first][lastPoint.second].second = true;
			if (createsWin(board, lastPoint)) {
				hasWon = true;
				if (--unwonBoards == 0) {
					cout << score(board) * n << enl;
					return 0;
				}
			}
		}
	}
}
