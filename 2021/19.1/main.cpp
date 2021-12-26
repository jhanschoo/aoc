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
using szt = std::size_t;

constexpr szt DIMS{3};
constexpr szt GRP_O_SZ{24};
constexpr szt GUARANTEED_OVERLAPS{12};

using coords = array<ll, DIMS>;
using rotm_t = array<coords, DIMS>;
using scanner_t = set<coords>;

constexpr rotm_t mul(const rotm_t &vl, const rotm_t &vr) {
	rotm_t res;
	for (szt i{0}; i < DIMS; ++i) {
		for (szt j{0}; j < DIMS; ++j) {
			res[i][j] = 0;
			for (szt k{0}; k < DIMS; ++k) {
				res[i][j] += vl[i][k] * vr[k][j];
			}
		}
	}
	return res;
}

constexpr coords mul(const rotm_t &rotm, const coords &c) {
	coords res;
	for (szt i{0}; i < DIMS; ++i) {
		res[i] = 0;
		for (szt j{0}; j < DIMS; ++j) {
			res[i] += rotm[i][j] * c[j];
		}
	}
	return res;
}

constexpr coords sub(const coords &l, const coords &r) {
	coords res;
	for (szt i{0}; i < DIMS; ++i) {
		res[i] = l[i] - r[i];
	}
	return res;
}

constexpr ll L1(const coords &c) {
	ll res{0};
	for (szt i{0}; i < DIMS; ++i) {
		res += abs(c[i]);
	}
	return res;
}

constexpr rotm_t E{{
	{1, 0, 0},
	{0, 1, 0},
	{0, 0, 1}
}}, S1{{ // (x0, y0, z0) -> (z0, -y0, x0)
	{0, 0, 1},
	{0, -1, 0},
	{1, 0, 0}
}}, S2{{ // (x0, y0, z0) -> (-z0, y0, -x0)
	{0, 1, 0},
	{1, 0, 0},
	{0, 0, -1}
}}, S3{{ // (x0, y0, z0) -> (-z0, -y0, -x0)
	{0, 0, -1},
	{0, -1, 0},
	{-1, 0, 0}
}}, S2S3{mul(S2, S3)}, S3S2{mul(S3, S2)}, S3S1{mul(S3, S1)}, S1S2{mul(S1, S2)}, S2S1{mul(S2, S1)},
	S2S3S2{mul(S2, S3S2)}, S1S2S3{mul(S1, S2S3)}, S3S1S2{mul(S3, S1S2)}, S2S3S1{mul(S2, S3S1)}, S3S2S1{mul(S3, S2S1)}, S1S2S1{mul(S1, S2S1)},
	S1S2S3S2{mul(S1, S2S3S2)}, S2S3S1S2{mul(S2, S3S1S2)}, S1S2S3S1{mul(S1, S2S3S1)}, S2S3S2S1{mul(S2, S3S2S1)}, S1S3S2S1{mul(S1, S3S2S1)},
	S2S1S2S3S2{mul(S2, S1S2S3S2)}, S1S2S3S2S1{mul(S1, S2S3S2S1)}, S2S3S1S2S1{mul(S2S3S1S2, S1)},
	S1S2S3S1S2S1{mul(S1, S2S3S1S2S1)};

constexpr array<rotm_t, GRP_O_SZ> GRP_O{
	E,
	S1, S2, S3,
	S2S3, S3S2, S3S1, S1S2, S2S1,
	S2S3S2, S1S2S3, S3S1S2, S2S3S1, S3S2S1, S1S2S1,
	S1S2S3S2, S2S3S1S2, S1S2S3S1, S2S3S2S1, S1S3S2S1,
	S2S1S2S3S2, S1S2S3S2S1, S2S3S1S2S1,
	S1S2S3S1S2S1
};

bool compatible(const scanner_t &l, const scanner_t &r) {
	scanner_t intersection;
	set_intersection(l.begin(), l.end(), r.begin(), r.end(), inserter(intersection, intersection.begin()));
	return intersection.size() >= GUARANTEED_OVERLAPS;
}

// translation is vector from l to r
pair<bool, scanner_t> compatible_with_translation(const scanner_t &l, const scanner_t &r, const coords &translation) {
	scanner_t translated_r;
	for (const auto &c : r) {
		translated_r.insert(sub(c, translation));
	}
	return {compatible(l, translated_r), translated_r};
}

// simplifying assumption not in problem statement: once there are 12 overlaps,
//   between beacon maps, the translation is correct.
//   This means
//   - there is no need for checking pairwise maps for conflicts withing scanners' detection range once 12 overlaps are found
//   - there is no backtracking needed, since we assume we do not have a situation where a,b,c are compatible in multiple ways, and d is compatible with a,b,c in only some of those multiple ways.
int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);
	string s;
	set<scanner_t> unlocated;
	vector<scanner_t> located;
	while (getline(cin, s)) {
		scanner_t scanner;
		while (cin.peek() != '\n' && cin.peek() != char_traits<char>::eof() && cin) {
			coords c;
			for (szt i{0}; i < DIMS; ++i) {
				cin >> c[i];
				cin.get();
			}
			scanner.insert(c);
		}
		cin.get();
		unlocated.insert(scanner);
	}
	auto first{unlocated.begin()};
	scanner_t beacons{first->begin(), first->end()};
	located.PB(*first);
	unlocated.erase(first);
	while (!unlocated.empty()) {
		bool found{false};
		for (auto it{unlocated.cbegin()}; it != unlocated.cend(); ++it) {
			const auto &unoriented_scanner{*it};
			scanner_t oriented_scanner;
			for (auto oit{GRP_O.cbegin()}; !found && oit != GRP_O.cend(); ++oit) {
				oriented_scanner.clear();
				for_each(unoriented_scanner.begin(), unoriented_scanner.end(), [&](const coords &c) {
					oriented_scanner.insert(mul(*oit, c));
				});
				for (auto jt{located.cbegin()}; !found && jt < located.cend(); ++jt) {
					szt i{0}, translations_to_check{oriented_scanner.size() - (GUARANTEED_OVERLAPS - 1)};
					for (auto iit{oriented_scanner.begin()}; !found && i < translations_to_check; ++i, ++iit) {
						for (auto jjt{jt->begin()}; !found && jjt != jt->end(); ++jjt) {
							// translation is vector from oriented beacon to unoriented becon pos
							auto translation{sub(*iit, *jjt)};
							auto [compatible, translated_scanner]{compatible_with_translation(*jt, oriented_scanner, translation)};
							if (compatible) {
								found = true;
								beacons.insert(translated_scanner.begin(), translated_scanner.end());
								located.PB(translated_scanner); // jt, jjt invalidated
							}
						}
					}
				}
			}
			if (found) {
				unlocated.erase(it); // it invalidated?
				break;
			}
		}
		if (!found) {
			cout << "No solution" << endl;
			return 0;
		}
	}
	cout << "No. of beacons: " << beacons.size() << enl;
}
