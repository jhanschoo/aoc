#include <bits/stdc++.h>

namespace {

    using lli = long long;

    auto DIR_TOTAL_INCL_UPPER_BOUND = 100000LL;
}

/**
 * During execution, keys that point to subpaths of `path` are deleted, their values (denoting their (total) sizes)
 * accumulated, and a new key `path` inserted containing the total size of `path`. `path` remains unchanged, and
 * in normal operation in certain scenarios, you would want to `path.pop_back()` after this call.
 * @param sizes is a map from paths to their total sizes (in case of directories), such that the sum of all values
 *      equals to the total size of the root directory
 * @param path specifies a directory that is not the root directory
 * @return the total size of the directory unwound. `sizes` and `path` are mutated as described above.
 */
lli consolidate(std::map<std::vector<std::string>, lli> &sizes, std::vector<std::string> &path) {
    auto sum = 0LL;
    auto it = sizes.lower_bound(path);
    // sentinel path
    path.emplace_back("|");
    auto end = sizes.lower_bound(path);
    while (it != end) {
        sum += it->second;
        auto prev = it;
        ++it;
        sizes.erase(prev);
    }
    path.pop_back();
    sizes[path] = sum;
    return sum;
}

lli unwind_all(std::map<std::vector<std::string>, lli> &sizes, std::vector<std::string> &path) {
    auto dir_totals = 0LL;
    while (!path.empty()) {
        auto curr_dir_total = consolidate(sizes, path);
        if (curr_dir_total <= DIR_TOTAL_INCL_UPPER_BOUND) {
            dir_totals += curr_dir_total;
        }
        path.pop_back();
    }
    auto curr_dir_total = consolidate(sizes, path);
    if (curr_dir_total <= DIR_TOTAL_INCL_UPPER_BOUND) {
        dir_totals += curr_dir_total;
    }
    return dir_totals;
}

void diagnose_sizes(std::map<std::vector<std::string>, lli> &sizes) {
    for (auto &&[key, value]: sizes) {
        for (auto &&s: key) {
            std::cout << s << '/';
        }
        std::cout << ' ' << value << std::endl;
    }
}

int main() {
    auto path = std::vector<std::string>{};
    auto sizes = std::map<std::vector<std::string>, lli>{};
    auto dir_totals = 0LL;

    std::string ignore_1, ignore_2, dir;
    while (std::cin >> ignore_1 >> ignore_2 >> dir) {
        // expect $ cd <dir>
        if (dir == "..") {
            auto curr_dir_total = consolidate(sizes, path);
            if (curr_dir_total <= DIR_TOTAL_INCL_UPPER_BOUND) {
                dir_totals += curr_dir_total;
            }
            path.pop_back();
            continue;
        } else if (dir == "/") {
            // noop, should appear only as first line in file
        } else {
            path.emplace_back(dir);
        }
        // discard \n$ ls\n
        std::cin >> ignore_1 >> ignore_2;
        std::cin.get();
        while (std::cin && std::cin.peek() != '$') {
            // while still listing directory items
            if (std::cin.peek() == 'd') {
                // dir
                // we ignore such lines
                std::cin >> ignore_1 >> ignore_2;
                std::cin.get();
            } else {
                auto size = 0LL;
                auto file = std::string{};
                std::cin >> size >> file;
                std::cin.get();
                path.push_back(file);
                sizes[path] = size;
                path.pop_back();
            }
        }
    }
    dir_totals += unwind_all(sizes, path);
    std::cout << dir_totals << std::endl;

    return 0;
}