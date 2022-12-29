#include <bits/stdc++.h>
#include <range/v3/all.hpp>

int main() {
	std::vector<std::string> chart_strs;
	std::string line;
	while (std::cin >> line) {
        chart_strs.push_back(line);
	}
	auto num_rows = chart_strs.size();
	auto num_cols = chart_strs[0].size();

    auto chart = std::valarray<char>(num_rows * num_cols);
    for (std::size_t i = 0; i < num_rows; ++i) {
        for (std::size_t j = 0; j < num_cols; ++j) {
            chart[i * num_cols + j] = chart_strs[i][j];
        }
    }

    auto visible = std::valarray<bool>(false, num_rows * num_cols);
    auto regimes = std::array{
        std::tuple{num_rows, num_cols, static_cast<size_t>(1), num_cols},
        std::tuple{num_cols, num_rows, num_cols, static_cast<size_t>(1),},
    };
    for (auto [size, count, offset_mul, stride] : regimes) {
        auto max_heights_pair = std::array{std::valarray<char>('\0', size), std::valarray<char>('\0', size)};
        for (std::size_t ind = 0; ind < count; ++ind) {
            auto js = std::array{ind, count - ind - 1};
            for (auto&& [j, max_heights] : ranges::views::zip(js, max_heights_pair)) {
                auto sl = std::slice(j * offset_mul, size, stride);
                auto current_col = std::valarray(chart[sl]);
                auto current_visible = current_col > max_heights;
                visible[sl] = current_visible || visible[sl];

                max_heights[current_visible] = current_col[current_visible];
            }
        }
    }
	int sum = 0;
	for (size_t i = 0; i < num_rows * num_cols; ++i) {
			sum += visible[i];
	}
	std::cout << sum << std::endl;
}
