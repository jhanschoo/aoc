#include <bits/stdc++.h>

void move_tail(auto &tail, auto &head) {
    auto &[x, y] = tail;
    auto &[hx, hy] = head;
    // distance eastward the head is from the tail
    auto dx = hx - x;
    // distance northward the head is from the tail
    auto dy = hy - y;
    if (dx == 0) {
        // if same longitude
        if (dy > 1) {
            // head north of tail
            ++y;
        } else if (dy < -1) {
            // head south of tail
            --y;
        }
    } else if (dy == 0) {
        // if same latitude
        if (dx > 1) {
            // head east of tail
            ++x;
        } else if (dx < -1) {
            // head west of tail
            --x;
        }
    } else if (dx > 0 && dy > 0 && dx + dy > 2) {
        // head northeast of tail
        ++x;
        ++y;
    } else if (dx > 0 && dy < 0 && dx - dy > 2) {
        // head northwest of tail
        ++x;
        --y;
    } else if (dx < 0 && dy > 0 && dy - dx > 2) {
        // head southeast of tail
        --x;
        ++y;
    } else if (dx < 0 && dy < 0 && dx + dy < -2) {
        // head southwest of tail
        --x;
        --y;
    }
}

int main() {
    using ll = long long;
    auto seen = std::set{std::pair{0ll, 0ll}};
    auto rope = std::array<std::pair<ll, ll>, 10>{std::pair{0ll, 0ll}};
    char direction;
    int steps;
    while (std::cin >> direction >> steps) {
        for (int s = 0; s < steps; ++s) {
            switch (direction) {
                case 'U':
                    ++rope[0].second;
                    break;
                case 'D':
                    --rope[0].second;
                    break;
                case 'R':
                    ++rope[0].first;
                    break;
                case 'L':
                default:
                    --rope[0].first;
                    break;
            }
            for (size_t i = 0; i < rope.size() - 1; ++i) {
                move_tail(rope[i + 1], rope[i]);
            }
            seen.insert(rope[rope.size() - 1]);
        }
    }
    std::cout << seen.size() << std::endl;
}
