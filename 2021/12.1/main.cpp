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
	const auto &[i, j] = p;
	++field[i][j];
	if (field[i][j] > 9) {
		field[i][j] = -10;
		to_flash.PB(p);
	}
}

int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);
	map<string, vector<string>> edge_list;
	{
		string s;
		while (cin >> s) {
			auto pos = s.find('-');
			auto s1{s.substr(0, pos)}, s2{s.substr(pos + 1)};
			if (!(s2 == "start" || s1 == "end")) {
				edge_list[s1].PB(s2);
			}
			if (!(s1 == "start" || s2 == "end")) {
				edge_list[s2].PB(s1);
			}
		}
	}
	int paths{0};
	set<string> visited;
	vector<pair<string, std::size_t>> to_visit{{"start", 0}};
	while (!to_visit.empty()) {
		auto &[node, i]{to_visit.back()};
		if (i >= edge_list[node].size()) {
			visited.erase(node);
			to_visit.pop_back();
			continue;
		}
		auto &next{edge_list[node][i]};
		++i;
		if (next == "end") {
			++paths;
		} else if (visited.count(next) == 0) {
			if ('a' <= next[0] && next[0] <= 'z') {
				visited.insert(next);
			}
			to_visit.PB({next, 0});
		}
	}
	cout << paths << enl;
}
