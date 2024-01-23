#include <bits/stdc++.h>

auto DIGITS = "0123456789";

char neighboringSymbol(const std::vector<std::string> &schematic, std::size_t row, std::size_t start_col, std::size_t end_col) {
    if (start_col != 0) {
        --start_col;
        if (schematic[row][start_col] != '.') {
            return schematic[row][start_col];
        }
    }
    if (end_col != schematic[row].size()) {
        ++end_col;
        if (schematic[row][end_col - 1] != '.') {
            return schematic[row][end_col - 1];
        }
    }

    if (row != 0) {
        auto sym = schematic[row - 1].find_first_not_of('.', start_col);
        if (sym < end_col) {
            return schematic[row - 1][sym];
        }
    }

    if (row != schematic.size() - 1) {
        auto sym = schematic[row + 1].find_first_not_of('.', start_col);
        if (sym < end_col) {
            return schematic[row + 1][sym];
        }
    }
    return '.';
}

int main() {
    std::string l;
    std::vector<std::string> schematic;
    while (std::getline(std::cin, l) && !l.empty()) {
        schematic.push_back(l);
    }

    auto part_num_sum = 0;
    for (std::size_t row = 0; row < schematic.size(); ++row) {
        auto col = schematic[row].find_first_of(DIGITS);
        for (typeof(col) end; col != std::string::npos; col = schematic[row].find_first_of(DIGITS, end)) {
            end = schematic[row].find_first_not_of(DIGITS, col);
            if (end == std::string::npos) {
                end = schematic[row].size();
            }
            auto sym = neighboringSymbol(schematic, row, col, end);
            if (sym != '.') {
                auto part_num = std::stoi(std::string{schematic[row].substr(col, end - col)});
                part_num_sum += part_num;
            }
        }
    }
    std::cout << part_num_sum << std::endl;
}
