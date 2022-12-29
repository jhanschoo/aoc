#include <bits/stdc++.h>

int main() {
	std::vector<std::string> treemap;
	std::string line;
	while (std::cin >> line) {
		treemap.push_back(line);
	}
	auto num_rows = treemap.size();
	auto num_cols = treemap[0].size();
	std::vector<std::vector<int>> scenic_left, scenic_right, scenic_up, scenic_down;
	for (size_t i = 0; i < num_rows; ++i) {
		scenic_left.push_back(std::vector<int>(num_cols, -1));
		scenic_right.push_back(std::vector<int>(num_cols, -1));
	}
	for (size_t j = 0; j < num_cols; ++j) {
		scenic_up.push_back(std::vector<int>(num_rows, -1));
		scenic_down.push_back(std::vector<int>(num_rows, -1));
	}
	for (size_t i = 0; i < num_rows; ++i) {
		auto decreasing = std::vector<std::pair<size_t, char>>{};
		for (size_t j = 0; j < num_cols; ++j) {
			auto& score_left = scenic_left[i][j];
			score_left = j;
			for (auto it = decreasing.begin(); it != decreasing.end(); ++it) {
				const auto& [index, height] = *it;
				if (height >= treemap[i][j]) {
					score_left = j - index;
				} else {
					break;
				}
			}
			while (!decreasing.empty() && decreasing.back().second <= treemap[i][j]) {
				decreasing.pop_back();
			}
			decreasing.push_back({j, treemap[i][j]});
		}
		decreasing.resize(0);
		for (size_t j_cpl = 0; j_cpl < num_cols; ++j_cpl) {
			auto j = num_cols - j_cpl - 1;
			auto& score_right = scenic_right[i][j];
			score_right = j_cpl;
			for (auto it = decreasing.begin(); it != decreasing.end(); ++it) {
				const auto& [index, height] = *it;
				if (height >= treemap[i][j]) {
					score_right = index - j;
				} else {
					break;
				}
			}
			while (!decreasing.empty() && decreasing.back().second <= treemap[i][j]) {
				decreasing.pop_back();
			}
			decreasing.push_back({j, treemap[i][j]});
		}
	}
	for (size_t j = 0; j < num_cols; ++j) {
		auto decreasing = std::vector<std::pair<size_t, char>>{};
		for (size_t i = 0; i < num_rows; ++i) {
			auto& score_up = scenic_up[i][j];
			score_up = i;
			for (auto it = decreasing.begin(); it != decreasing.end(); ++it) {
				const auto& [index, height] = *it;
				if (height >= treemap[i][j]) {
					score_up = i - index;
				} else {
					break;
				}
			}
			while (!decreasing.empty() && decreasing.back().second <= treemap[i][j]) {
				decreasing.pop_back();
			}
			decreasing.push_back({i, treemap[i][j]});
		}
		decreasing.resize(0);
		for (size_t i_cpl = 0; i_cpl < num_rows; ++i_cpl) {
			auto i = num_rows - i_cpl - 1;
			auto& score_down = scenic_down[i][j];
			score_down = i_cpl;
			for (auto it = decreasing.begin(); it != decreasing.end(); ++it) {
				const auto& [index, height] = *it;
				if (height >= treemap[i][j]) {
					score_down = index - i;
				} else {
					break;
				}
			}
			while (!decreasing.empty() && decreasing.back().second <= treemap[i][j]) {
				decreasing.pop_back();
			}
			decreasing.push_back({i, treemap[i][j]});
		}
	}
	int max = 0;
	for (size_t i = 0; i < num_rows; ++i) {
		for (size_t j = 0; j < num_cols; ++j) {
			max = std::max(max, scenic_left[i][j] * scenic_right[i][j] * scenic_up[i][j] * scenic_down[i][j]);
		}
	}
	std::cout << max << std::endl;
}
