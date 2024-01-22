#include <bits/stdc++.h>

enum Color {
    RED,
    GREEN,
    BLUE
};

enum Next {
    SAME_ROUND,
    SAME_GAME,
    NEXT_GAME
};

std::pair<Color, Next> parseColorNext(const std::string& str) {
    auto color = Color{};
    auto next = Next{};
    if (str.starts_with("red")) {
        color = RED;
    } else if (str.starts_with("green")) {
        color = GREEN;
    } else if (str.starts_with("blue")) {
        color = BLUE;
    } else {
        throw std::runtime_error("invalid color");
    }
    if (str.ends_with(",")) {
        next = SAME_ROUND;
    } else if (str.ends_with(";")) {
        next = SAME_GAME;
    } else {
        next = NEXT_GAME;
    }
    return {color, next};
}

int main() {
    auto powerSum = 0;
    for (auto id = 1; std::cin; ++id) {
        auto n_red = 0, n_green = 0, n_blue = 0;

        auto line = std::string{}, colorNextStr = std::string{};
        std::getline(std::cin, line);
        if (line.empty()) {
            break;
        }
        std::istringstream iss{line};
        iss >> colorNextStr; iss >> colorNextStr; // ignore two words
        auto n = 0;
        while (iss >> n >> colorNextStr) {
            auto [color, next] = parseColorNext(colorNextStr);
            switch (color) {
                case RED:
                    n_red = std::max(n_red, n);
                    break;
                case GREEN:
                    n_green = std::max(n_green, n);
                    break;
                case BLUE:
                    n_blue = std::max(n_blue, n);
                    break;
            }
        }
        powerSum += n_red * n_blue * n_green;
    }
    std::cout << powerSum << std::endl;
}
