#include <bits/stdc++.h>

int main() {
    std::vector<std::string> chartLines;
    std::string line;
    while (std::cin >> line) {
        chartLines.push_back(line);
    }
    auto num_rows = chartLines.size();
    auto num_cols = chartLines[0].size();
    auto cells = num_rows * num_cols;

    auto chart = std::valarray<char>(num_rows * num_cols);
    for (std::size_t i = 0; i < num_rows; ++i) {
        for (std::size_t j = 0; j < num_cols; ++j) {
            chart[i * num_cols + j] = chartLines[i][j];
        }
    }

    auto regimes = std::array{
            // row-wise ltr
            std::tuple{num_rows, // num_maj
                       num_cols, // num_min
                       +[](std::size_t num_rows, std::size_t num_cols, std::size_t maj, std::size_t min) {
                           return maj * num_cols + min;
                       }, std::valarray(std::size_t{}, cells)},
            // row-wise rtl
            std::tuple{num_rows, num_cols,
                       +[](std::size_t num_rows, std::size_t num_cols, std::size_t maj, std::size_t min) {
                           return maj * num_cols + (num_cols - min - 1);
                       }, std::valarray(std::size_t{}, cells)},
            // col-wise utd
            std::tuple{num_cols, num_rows,
                       +[](std::size_t num_rows, std::size_t num_cols, std::size_t maj, std::size_t min) {
                           return maj + min * num_cols;
                       }, std::valarray(std::size_t{}, cells)},
            // col-wise dtu
            std::tuple{num_cols, num_rows,
                       +[](std::size_t num_rows, std::size_t num_cols, std::size_t maj, std::size_t min) {
                           return maj + (num_rows - min - 1) * num_cols;
                       }, std::valarray(std::size_t{}, cells)},};

    for (auto &&[num_maj, num_min, abs_map, scenic]: regimes) {
        for (std::size_t maj = 0; maj < num_maj; ++maj) {
            auto decreasing = std::vector<std::pair<std::size_t, char>>{};
            for (std::size_t min = 0; min < num_min; ++min) {
                auto abs_i = abs_map(num_rows, num_cols, maj, min);
                auto current_height = chart[abs_i];
                auto &score = scenic[abs_i];
                score = min;
                for (const auto &[index, height]: decreasing) {
                    if (height >= current_height) {
                        score = min - index;
                    } else {
                        break;
                    }
                }
                while (!decreasing.empty() && decreasing.back().second <= current_height) {
                    decreasing.pop_back();
                }
                decreasing.emplace_back(min, current_height);
            }
        }
    }

    std::cout << (std::get<3>(regimes[0]) * std::get<3>(regimes[1]) * std::get<3>(regimes[2]) *
                  std::get<3>(regimes[3])).max() << std::endl;
}
