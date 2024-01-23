#include <bits/stdc++.h>

int main() {
    std::string line;
    std::map<int, int> card_extra_copies;
    int card = 0;
    while (std::getline(std::cin, line) && !line.empty()) {
        ++card;
        int copies = card_extra_copies[card] + 1;

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
        std::cout << card << " " << copies << " " << num_matches << std::endl;
        for (int i = 1; i <= num_matches; ++i) {
            card_extra_copies[card + i] += copies;
        }
    }
    auto total_copies = card;
    for (const auto &[_card, copies]: card_extra_copies) {
        total_copies += copies;
    }
    std::cout << total_copies << std::endl;
}
