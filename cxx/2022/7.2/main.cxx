#include <bits/stdc++.h>
#include <range/v3/all.hpp>

namespace {

    using lli = long long;

    auto USED_INCL_UPPER_BOUND = 40000000LL;
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
void consolidate(std::map<std::vector<std::string>, lli> &sizes, std::vector<std::string> &path,
                 std::map<std::vector<std::string>, lli> &folder_sizes) {
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
    folder_sizes[path] = sum;
}

lli unwind_all(std::map<std::vector<std::string>, lli> &sizes, std::vector<std::string> &path,
               std::map<std::vector<std::string>, lli> &folder_sizes) {
    while (!path.empty()) {
        consolidate(sizes, path, folder_sizes);
        path.pop_back();
    }
    consolidate(sizes, path, folder_sizes);
    return folder_sizes[std::vector<std::string>{}];
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
    auto sizes = std::map<std::vector<std::string>, lli>{}, folder_sizes = std::map<std::vector<std::string>, lli>{};

    std::string ignore_1, ignore_2, dir;
    while (std::cin >> ignore_1 >> ignore_2 >> dir) {
        // expect $ cd <dir>
        if (dir == "..") {
            consolidate(sizes, path, folder_sizes);
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
    auto root_size = unwind_all(sizes, path, folder_sizes);
    auto lower_bound = root_size - USED_INCL_UPPER_BOUND;
    auto m = ranges::min(folder_sizes | std::views::values |
                         std::views::filter(
                                 [&lower_bound](auto &sz) { return sz >= lower_bound; }));
    std::cout << m << std::endl;
    return 0;
}