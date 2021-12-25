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

constexpr szt RM_SZ{4};

using hallway_t = array<int, 11>;
using side_t = array<int, RM_SZ>;
using sides_t = array<side_t, 4>;
using state_t = pair<hallway_t, sides_t>;
using memo_t = map<state_t, ll>;

constexpr int EMPTY{-1};

constexpr state_t empty_state_init() {
	state_t state;
	auto &[hallway, sides] = state;
	fill(begin(hallway), end(hallway), EMPTY);
	for (auto &side : sides) {
		fill(begin(side), end(side), EMPTY);
	}
	return state;
}

constexpr state_t EMPTY_STATE{empty_state_init()};

constexpr state_t DEST_STATE{
	{{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
	{{
		{0, 0, 0, 0},
		{1, 1, 1, 1},
		{2, 2, 2, 2},
		{3, 3, 3, 3}
	}}
};

inline bool is_dest_state(const state_t &state) {
	return state.second == DEST_STATE.second;
}

constexpr inline ll cost_to_move(const int& amphipod, const ll& steps) {
	switch (amphipod) {
		case 0: return steps;
		case 1: return 10 * steps;
		case 2: return 100 * steps;
		case 3: return 1000 * steps;
		default: return -1;
	}
}

constexpr inline szt hallway_of_side(const szt &side_i) {
	return side_i * 2 + 2;
}

inline state_t read_state() {
	state_t state{EMPTY_STATE};
	auto &[hallway, sides]{state};
	char c;
	szt i{0}, j{0};
	while (cin >> c) {
		if ('A' <= c && c <= 'D') {
			sides[i++][j] = c - 'A';
			if (i == 4) {
				i = 0;
				j++;
			}
		}
	}
	return state;
}

void print_state(const state_t &state) {
	auto &[hallway, sides]{state};
	for (szt i{0}; i < hallway.size(); ++i) {
		cout << static_cast<char>(hallway[i] + 'a');
	}
	cout << '\n';
	for (szt j{0}; j < RM_SZ; ++j) {
		cout << ' ';
		for (szt i{0}; i < 4; ++i) {
			cout << ' ' << static_cast<char>(sides[i][j] + 'a');
		}
		cout << '\n';
	}
}

inline pair<state_t, ll> move_to_hallway(const state_t &state, szt hallway_i, szt side_i) {
	// bounds checking
	if (hallway_i > 10 || hallway_i == 2 || hallway_i == 4 || hallway_i == 6 || hallway_i == 8 || side_i > 3) {
		return {state, -1};
	}
	const auto &[hallway, sides]{state};
	const auto &side{sides[side_i]};
	// prune moving from rooms that only contain destination amphipods
	if (all_of(side.begin(), side.end(), [&](int apod) { return apod == EMPTY || apod == static_cast<int>(side_i); })) {
		return {state, -1};
	}
	// ensure path is clear
	auto [l, r]{pair{hallway_i, hallway_of_side(side_i)}};
	if (l > r) {
		swap(l, r);
	}
	if (!all_of(hallway.begin() + l, hallway.begin() + r + 1, [](int apod) { return apod == EMPTY; })) {
		return {state, -1};
	}
	// compute steps
	auto amphipod_i{find_if(side.begin(), side.end(), [](auto i) { return static_cast<int>(i) != EMPTY; }) - side.begin()};
	auto amphipod{side[amphipod_i]};
	const ll steps{static_cast<ll>(r - l + 1 + amphipod_i)};
	const ll cost{cost_to_move(amphipod, steps)};
	// move amphipod
	auto next_state{state};
	auto &[next_hallway, next_sides]{next_state};
	auto &next_side{next_sides[side_i]};
	next_side[amphipod_i] = EMPTY;
	next_hallway[hallway_i] = amphipod;
	return {next_state, cost};
}

inline pair<state_t, ll> move_to_side(const state_t &state, szt hallway_i) {
	const auto &[hallway, sides]{state};
	const auto &amphipod{hallway[hallway_i]};
	// bounds checking
	if (hallway_i > 10 || hallway_i == 2 || hallway_i == 4 || hallway_i == 6 || hallway_i == 8 || amphipod < 0 || amphipod > 3) {
		return {state, -1};
	}
	const szt &side_i{static_cast<szt>(amphipod)};
	const auto &side{sides[side_i]};
	// prune moving to rooms that contain more than destination amphipods
	if (!all_of(side.begin(), side.end(), [&](int apod) { return apod == EMPTY || apod == static_cast<int>(side_i); })) {
		return {state, -1};
	}
	// ensure path is clear
	auto [l, r]{pair{hallway_i, hallway_of_side(side_i)}};
	if (l > r) {
		swap(l, r);
	}
	if (!all_of(hallway.begin() + l, hallway.begin() + r + 1, [&amphipod](int apod) { return apod == EMPTY || apod == amphipod; })) {
		return {state, -1};
	}
	// compute steps
	auto amphipod_i{find_if(side.begin(), side.end(), [](auto i) { return static_cast<int>(i) != EMPTY; }) - 1 - side.begin()};
	const ll steps{static_cast<ll>(r - l + 1 + amphipod_i)};
	const ll cost{cost_to_move(amphipod, steps)};
	// move amphipod
	auto next_state{state};
	auto &[next_hallway, next_sides]{next_state};
	auto &next_side{next_sides[side_i]};
	next_side[amphipod_i] = amphipod;
	next_hallway[hallway_i] = EMPTY;
	return {next_state, cost};
}

int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);
	priority_queue<pair<int, state_t>, vector<pair<int, state_t>>, greater<tuple<int, state_t>>> pq;
	set<state_t> visited;
	pq.push({0, read_state()});
	while (!pq.empty()) {
		auto [cost, state]{pq.top()};
		pq.pop();
		// print_state(state);
		// cout << '\n';
		if (visited.count(state) != 0) {
			continue;
		}
		visited.insert(state);
		if (is_dest_state(state)) {
			cout << cost << '\n';
			return 0;
		}
		for (szt i{0}; i < 11; ++i) {
			{
				auto [next_state, next_cost]{move_to_side(state, i)};
				if (next_cost >= 0) {
					pq.push({cost + next_cost, next_state});
				}
			}
			for (szt j{0}; j < 4; ++j) {
				auto [next_state, next_cost]{move_to_hallway(state, i, j)};
				if (next_cost >= 0) {
					pq.push({cost + next_cost, next_state});
				}
			}
		}
	}
	return 0;
}
