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

using cuboid_t = array<pair<ll, ll>, 3>;
using aug_cuboid_t = pair<bool, cuboid_t>;

aug_cuboid_t intersect_cuboids(const cuboid_t &a, const cuboid_t &b) {
	bool nonempty{true};
	cuboid_t res;
	for (szt dim{0}; dim < res.size() && nonempty; ++dim) {
		auto &[min_dim, max_dim] = res[dim];
			min_dim = max(a[dim].F, b[dim].F);
			max_dim = min(a[dim].S, b[dim].S);
		nonempty = min_dim <= max_dim;
	}
	return {nonempty, res};
}

aug_cuboid_t inline read_cuboid() {
	string s;
	cin >> s;
	cuboid_t cuboid;
	for (auto &dim : cuboid) {
		char c;
		while ((cin >> c) && c != '=') {}
		cin >> dim.F;
		cin.get(); cin.get();
		cin >> dim.S;
	}
	cin.get();
	return {s == "on", cuboid};
}

constexpr ll cuboid_vol(const cuboid_t &c) {
	return (c[0].S - c[0].F + 1) * (c[1].S - c[1].F + 1) * (c[2].S - c[2].F + 1);
}

void inline print_cuboid(const cuboid_t &cuboid) {
	for (const auto &dim : cuboid) {
		cout << "(" << dim.F << ", " << dim.S << ") ";
	}
	cout << endl;
}

int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);
	vector<aug_cuboid_t> cuboids;
	ll vol{0};
	while (cin.peek() != std::char_traits<char>::eof()) {
		const auto aug_cuboid{read_cuboid()};
		const auto &[is_on, cuboid]{aug_cuboid};
		szt existing_len{cuboids.size()};
		for (szt i{0}; i < existing_len; ++i) {
			const auto [c_is_on, c]{cuboids[i]};
			const auto [intersects, res]{intersect_cuboids(cuboid, c)};
			if (intersects) {
				cuboids.PB({!c_is_on, res});
				vol += cuboid_vol(res) * (c_is_on ? -1 : 1);
			}
		}
		if (is_on) {
			cuboids.PB(aug_cuboid);
			vol += cuboid_vol(cuboid);
		}
		cout << vol << enl;
	}
	return 0;
}
