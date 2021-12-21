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

array<std::size_t, 10> universe_multipliers{0, 0, 0, 1, 3, 6, 7, 6, 3, 1};

int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);
	// min_score, max_score, min_next, p1_next, p1pos, p2pos
	// 21 * 24 * 2 * 2 * 10 * 10
	array<
		array<
			array<
				array<
					array<
						array<
							ll
							, 10
						>, 10
					>, 2
				>, 2
			>, 21
		>, 21
	> state_counts{0LL};
	std::size_t p1initpos, p2initpos;
	char c;
	while ((cin >> c) && c != ':') {}
	cin.get();
	cin >> p1initpos;
	while ((cin >> c) && c != ':') {}
	cin.get();
	cin >> p2initpos;
	ll p1wins{0LL}, p2wins{0LL};
	state_counts[0][0][0][0][p1initpos-1][p2initpos-1] = 1;
	for (std::size_t min_score{0}; min_score < 21; ++min_score) {
		for (std::size_t max_score{0}; max_score < 21; ++max_score) {
			for (std::size_t min_next{0}; min_next < 2; ++min_next) {
				for (std::size_t p1_next{0}; p1_next < 2; ++p1_next) {
					for (std::size_t p1pos{0}; p1pos < 10; ++p1pos) {
						for (std::size_t p2pos{0}; p2pos < 10; ++p2pos) {
							auto is_min_next{min_next == 0}, is_p1_next{p1_next == 0};
							auto score{is_min_next ? min_score : max_score};
							auto pos{is_p1_next ? p1pos : p2pos};
							auto counts = state_counts[min_score][max_score][min_next][p1_next][p1pos][p2pos];
							for (std::size_t moves{3}; moves <= 9; ++moves) {
								auto next_pos{(pos + moves) % 10};
								auto next_score{score + next_pos + 1};
								auto multiplied_counts{counts * universe_multipliers[moves]};
								if (next_score > 20) {
									if (is_p1_next) {
										p1wins += multiplied_counts;
									} else {
										p2wins += multiplied_counts;
									}
									continue;
								}
								if (is_min_next && next_score > max_score) {
									state_counts[max_score][next_score][0][1 - p1_next][is_p1_next ? next_pos : p1pos][is_p1_next ? p2pos : next_pos]
										+= multiplied_counts;
								} else {
									state_counts[
										is_min_next ? next_score : min_score
									][
										is_min_next ? max_score : next_score
									][1 - min_next][1 - p1_next][is_p1_next ? next_pos : p1pos][is_p1_next ? p2pos : next_pos]
										+= counts * universe_multipliers[moves];
								}
							}
						}
					}
				}
			}
		}
	}
	cout << p1wins << " " << p2wins << enl;
}
