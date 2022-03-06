#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
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

constexpr std::size_t CHAR_E{1};
constexpr std::size_t CHAR_C{0};

struct RawMap {
	string CF;
	string ACF;
	string BCDF;
	array<string, 3> len5s;
};

int parseDigit(const array<char, 2> &map, string s) {
	switch (s.size()) {
		case 2: return 1;
		case 3: return 7;
		case 4: return 4;
		case 7: return 8;
		case 5: {
			if (s.find(map[CHAR_E]) != string::npos) return 2;
			if (s.find(map[CHAR_C]) != string::npos) return 3;
			return 5;
		}
		default: {
			// size == 6
			if (s.find(map[CHAR_C]) == string::npos) return 6;
			if (s.find(map[CHAR_E]) == string::npos) return 9;
			return 0;
		}
	}
	throw;
}

array<char, 2> getMap(const RawMap &r) {
	CAN [CF, ACF, BCDF, len5s] = r;
	char C{}, E{};
	array<char, 2> BD;
	set_difference(BCDF.begin(), BCDF.end(), CF.begin(), CF.end(), BD.begin());
	string ACDEG, ACDFG;
	for (CAN s : len5s) {
		array<char, 2> tmp{};
		if (set_intersection(s.begin(), s.end(), BD.begin(), BD.end(), tmp.begin()) - tmp.begin() == 2) {
			continue;
		}
		if (set_intersection(s.begin(), s.end(), CF.begin(), CF.end(), tmp.begin()) - tmp.begin() == 2) {
			ACDFG = s;
			continue;
		}
		C = tmp[0];
		ACDEG = s;
	}
	set_difference(ACDEG.begin(), ACDEG.end(), ACDFG.begin(), ACDFG.end(), &E);
	return array<char, 2>{C, E};
}

int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);
	int total{0};
	while (cin.peek() != std::char_traits<char>::eof()) {
		string s;
		array<vector<string>, 7> dispBySize{vector<string>(3)};
		RawMap r;
		auto it{r.len5s.begin()};
		while (cin >> s && s != "|") {
			sort(s.begin(), s.end());
			switch (s.size()) {
				case 2: r.CF = s; break;
				case 3: r.ACF = s; break;
				case 4: r.BCDF = s; break;
				case 5: *it++ = s; break;
			}
		}
		auto map{getMap(r)};
		int number{0};
		while (cin >> s) {
			number = number * 10 + parseDigit(map, s);
			if (cin.get() == '\n') {
				break;
			}
		}
		total += number;
	}
	cout << total << enl;
}
