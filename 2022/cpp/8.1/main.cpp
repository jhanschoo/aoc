#include <algorithm>
#include <ranges>
#include <vector>
#include <string>
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>

int main() {
	std::vector<std::string> treemap;
	std::string line;
	while (std::cin >> line) {
		treemap.push_back(line);
	}
	auto num_rows = treemap.size();
	auto num_cols = treemap[0].size();
	std::vector<std::vector<bool>> visible;
	for (size_t i = 0; i < num_rows; ++i) {
		visible.push_back(std::vector<bool>(num_cols, false));
	}
	for (size_t i = 0; i < num_rows; ++i) {
		int max_height = -1;
		for (size_t j = 0; j < num_cols; ++j) {
			if (treemap[i][j] > max_height) {
				max_height = treemap[i][j];
				visible[i][j] = true;
			}
		}
		max_height = -1;
		for (size_t j = num_cols; j > 0; --j) {
			if (treemap[i][j-1] > max_height) {
				max_height = treemap[i][j-1];
				visible[i][j-1] = true;
			}
		}
	}
	for (size_t j = 0; j < num_cols; ++j) {
		int max_height = -1;
		for (size_t i = 0; i < num_rows; ++i) {
			if (treemap[i][j] > max_height) {
				max_height = treemap[i][j];
				visible[i][j] = true;
			}
		}
		max_height = -1;
		for (size_t i = num_rows; i > 0; --i) {
			if (treemap[i-1][j] > max_height) {
				max_height = treemap[i-1][j];
				visible[i-1][j] = true;
			}
		}
	}
	int sum = 0;
	for (size_t i = 0; i < num_rows; ++i) {
		for (size_t j = 0; j < num_cols; ++j) {
			sum += visible[i][j];
		}
	}
	std::cout << sum << std::endl;
}
