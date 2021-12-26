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

char convolve_at(const vector<string> &image, const string &lookup, const char &background, ll x, ll y) {
	szt ind{0};
	for (ll i{x - 1}; i <= x + 1; ++i) {
		for (ll j{y - 1}; j <= y + 1; ++j) {
			char c{background};
			if (i >= 0 && j >= 0 && i < static_cast<ll>(image.size()) && j < static_cast<ll>(image[0].size())) {
				c = image[i][j];
			}
			ind = ind * 2 + (c == '.' ? 0 : 1);
		}
	}
	return lookup[ind];
}

void step(vector<string> &image, const string &lookup, char &background) {
	vector<string> next_image;
	for (ll i{-1}; i <= static_cast<ll>(image.size()); ++i) {
		string row;
		for (ll j{-1}; j <= static_cast<ll>(image[0].size()); ++j) {
			char c{convolve_at(image, lookup, background, i, j)};
			row.PB(c);
		}
		next_image.PB(row);
	}
	background = (background == '.') ? lookup[0] : lookup[511];
	image = next_image;
}

int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);
	string lookup;

	cin >> lookup;

	vector<string> image;
	string s;
	while (cin >> s) {
		image.PB(s);
	}
	char background{'.'};
	for (int i{0}; i < 50; ++i) {
		step(image, lookup, background);
	}
	int lit{0};
	for (const auto &s : image) {
		for (const auto &c : s) {
			if (c == '#') {
				++lit;
			}
		}
	}
	cout << lit << enl;
	return 0;
}
