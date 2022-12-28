#include <algorithm>
#include <ranges>
#include <vector>
#include <string>
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>

int main() {
	using ll = long long;
	std::vector<std::string> heightmap;
	std::string line;
	while (std::cin >> line) {
		heightmap.push_back(line);
	}
	for (auto&& row : heightmap) {
		std::cout << row << std::endl;
	}
	auto start = std::pair<size_t, size_t>{0, 0};
	auto goal = std::pair<size_t, size_t>{0, 0};
	for (size_t i = 0; i < heightmap.size(); ++i) {
		for (size_t j = 0; j < heightmap[i].size(); ++j) {
			if (heightmap[i][j] == 'S') {
				start = {i, j};
				heightmap[i][j] = 'a';
			}
			if (heightmap[i][j] == 'E') {
				goal = {i, j};
				heightmap[i][j] = 'z';
			}
		}
	}
	for (auto&& row : heightmap) {
		std::cout << row << std::endl;
	}
	auto frontier = std::vector{start};
	auto seen = std::set<std::pair<size_t, size_t>>{};
	int steps = 0;
	while (!frontier.empty()) {
		auto new_frontier = std::vector<std::pair<size_t, size_t>>{};
		for (auto&& p : frontier) {
			std::cout << p.first << " " << p.second << std::endl;
			if (p == goal) {
				std::cout << steps << std::endl;
				return 0;
			}
			if (seen.contains(p)) {
				continue;
			}
			seen.insert(p);
			auto [i, j] = p;
			auto current = heightmap[i][j];
			if (i > 0 && heightmap[i-1][j] - current <= 1) {
				new_frontier.push_back({i-1, j});
			}
			if (i < heightmap.size()-1 && heightmap[i+1][j] - current <= 1) {
				new_frontier.push_back({i+1, j});
			}
			if (j > 0 && heightmap[i][j-1] - current <= 1) {
				new_frontier.push_back({i, j-1});
			}
			if (j < heightmap[i].size()-1 && heightmap[i][j+1] - current <= 1) {
				new_frontier.push_back({i, j+1});
			}
		}
		frontier = new_frontier;
		++steps;
	}
	std::cout << "no path" << std::endl;
}
