#include <bits/stdc++.h>

int main() {
    using namespace std;

    ios::sync_with_stdio(0);
    cin.tie(0);

    auto maximum = 0, current = 0;
    auto s = string{};
    while (getline(cin, s)) {
        if (s.empty()) {
            current = 0;
        } else {
            current += stoi(s);
            maximum = max(current, maximum);
            current = 0;
        }
    }
    cout << maximum << endl;
}
