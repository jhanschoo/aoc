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

int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);
	vector<pair<int, int>> stack;
	vector<tuple<int, int, int>> processed;
	for (int i{0}; i < 14; ++i) {
		int zi, xi, yi;
		// input.txt is preprocessed to be last numbers of lines
		//   div z {zi} (4th line after inp w)
		//   add x {xi} (5th line after inp w)
		//   add y {yi} (15th line after inp w)
		cin >> zi >> xi >> yi;
		if (zi == 1) {
			stack.PB({i, yi});
		} else {
			auto [j, yj] = stack.back();
			stack.pop_back();
			processed.PB({j, i, yj + xi});
		}
	}
	sort(processed.begin(), processed.end());
	array<int, 14> ans;
	for (auto [i, j, d] : processed) {
		for (int ni{1}; ni < 10; ++ni) {
			auto nj{ni + d};
			if (0 < nj && nj <= 9) {
				ans[i] = ni;
				ans[j] = nj;
				break;
			}
		}
	}
	for (auto n : ans) {
		cout << n;
	}
	cout << enl;
	return 0;
}
