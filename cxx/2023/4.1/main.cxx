#include <bits/stdc++.h>

int main() {
    std::string line;
    int total_points = 0;
    while (std::getline(std::cin, line) && !line.empty()) {
        std::stringstream ss{line};
        std::string word;
        std::set<int> winning_numbers;
        // process header
        ss >> word; // "Card"
        ss >> word; // "<number>:"

        // process winning numbers
        while (ss >> word && word != "|") {
            winning_numbers.insert(std::stoi(word));
        }

        // process own numbers
        int num_matches = 0;
        while (ss >> word) {
            auto num = std::stoi(word);
            if (winning_numbers.contains(num)) {
                ++num_matches;
            }
        }
        if (num_matches == 0) {
            continue;
        }
        auto points = std::pow(2, num_matches - 1);
        total_points += static_cast<int>(points);
    }
    std::cout << total_points << std::endl;
}
