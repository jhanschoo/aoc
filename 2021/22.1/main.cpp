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

constexpr int INIT_MIN{-50};
constexpr int INIT_MAX{50};
constexpr int INIT_SIZE{INIT_MAX - INIT_MIN + 1};

tuple<bool, array<int, 6>> inline read_cuboid() {
	string s;
	cin >> s;
	array<int, 6> cuboid;
	for (std::size_t i{0}; i < 6; i += 2) {
		char c;
		while ((cin >> c) && c != '=') {}
		cin >> cuboid[i];
		cuboid[i] -= INIT_MIN;
		cin.get(); cin.get();
		cin >> cuboid[i+1];
		cuboid[i+1] -= INIT_MIN;
	}
	cin.get();
	return {s == "on", cuboid};
}

int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);
	array<array<array<bool, INIT_SIZE>, INIT_SIZE>, INIT_SIZE> field{false};
	while (cin.peek() != std::char_traits<char>::eof()) {
		auto [is_on, cuboid]{read_cuboid()};
		for (int x{max(0, cuboid[0])}; x <= min(INIT_SIZE - 1, cuboid[1]); ++x) {
			for (int y{max(0, cuboid[2])}; y <= min(INIT_SIZE - 1, cuboid[3]); ++y) {
				for (int z{max(0, cuboid[4])}; z <= min(INIT_SIZE - 1, cuboid[5]); ++z) {
					field[x][y][z] = is_on;
				}
			}
		}
		ll on_total{0};
		for (int x{0}; x < INIT_SIZE; ++x) {
			for (int y{0}; y < INIT_SIZE; ++y) {
				for (int z{0}; z < INIT_SIZE; ++z) {
					if (field[x][y][z]) {
						++on_total;
					}
				}
			}
		}
		cout << on_total << enl;
	}
	return 0;
}
