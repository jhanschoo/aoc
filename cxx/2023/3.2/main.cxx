#include <bits/stdc++.h>

auto DIGITS = "0123456789";

using Coords = std::pair<std::size_t, std::size_t>;

std::vector<Coords> neighboringGears(const std::vector<std::string> &schematic, std::size_t row, std::size_t start_col, std::size_t end_col) {
    std::vector<Coords> neighboring_gears;
    if (start_col != 0) {
        --start_col;
        if (schematic[row][start_col] == '*') {
            neighboring_gears.emplace_back(row, start_col);
        }
    }
    if (end_col != schematic[row].size()) {
        ++end_col;
        if (schematic[row][end_col - 1] == '*') {
            neighboring_gears.emplace_back(row, end_col - 1);
        }
    }

    if (row != 0) {
        for (auto col = start_col; col < end_col; ++col) {
            if (schematic[row - 1][col] == '*') {
                neighboring_gears.emplace_back(row - 1, col);
            }
        }
    }

    if (row != schematic.size() - 1) {
        for (auto col = start_col; col < end_col; ++col) {
            if (schematic[row + 1][col] == '*') {
                neighboring_gears.emplace_back(row + 1, col);
            }
        }
    }
    return neighboring_gears;
}

int main() {
    std::string l;
    std::vector<std::string> schematic;
    std::map<Coords, std::vector<int>> gear_to_part_nums;
    while (std::getline(std::cin, l) && !l.empty()) {
        schematic.push_back(l);
    }

    for (std::size_t row = 0; row < schematic.size(); ++row) {
        auto col = schematic[row].find_first_of(DIGITS);
        for (typeof(col) end; col != std::string::npos; col = schematic[row].find_first_of(DIGITS, end)) {
            // extract part number
            end = schematic[row].find_first_not_of(DIGITS, col);
            if (end == std::string::npos) {
                end = schematic[row].size();
            }
            auto part_num = std::stoi(std::string{schematic[row].substr(col, end - col)});

            // determine neighboring gears
            auto gear_coords = neighboringGears(schematic, row, col, end);
            for (const auto & coords: gear_coords) {
                gear_to_part_nums[coords].push_back(part_num);
            }
        }
    }

    // compute gear ratio sums
    int gear_ratio_sum = 0;
    for (const auto & [coords, part_nums]: gear_to_part_nums) {
        if (part_nums.size() != 2) {
            continue;
        }
        gear_ratio_sum += part_nums[0] * part_nums[1];
    }
    std::cout << gear_ratio_sum << std::endl;
}
