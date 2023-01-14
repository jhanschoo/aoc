#include <bits/stdc++.h>
#include <range/v3/all.hpp>

namespace {
    using ull = unsigned long long;

    constexpr auto DIAGNOSTIC_MONKEYS = 4u;

    constexpr auto NUM_ROUNDS = 10000u;

    constexpr auto DIAGNOSTIC_OPS = std::array{
            +[](ull old) { return old * 19; },
            +[](ull old) { return old + 6; },
            +[](ull old) { return old * old; },
            +[](ull old) { return old + 3; },
    };

    constexpr auto DIAGNOSTIC_DIVISORS = std::array{ 23ull, 19ull, 13ull, 17ull };
    constexpr auto DIAGNOSTIC_MODULUS = 23ull * 19ull * 13ull * 17ull;

    constexpr auto DIAGNOSTIC_TARGETS = std::array{
            std::pair{ 2, 3 },
            std::pair{ 2, 0 },
            std::pair{ 1, 3 },
            std::pair{ 0, 1 },
    };

    const auto TEST_MONKEYS = 8u;

    constexpr auto TEST_OPS = std::array{
            +[](ull old) { return old * 2; },
            +[](ull old) { return old + 3; },
            +[](ull old) { return old + 6; },
            +[](ull old) { return old + 5; },
            +[](ull old) { return old + 8; },
            +[](ull old) { return old * 5; },
            +[](ull old) { return old * old; },
            +[](ull old) { return old + 4; },
    };

    constexpr auto TEST_DIVISORS = std::array{ 17ull, 13ull, 19ull, 7ull, 11ull, 3ull, 2ull, 5ull };
    constexpr auto TEST_MODULUS = 17ull * 13ull * 19ull * 7ull * 11ull * 3ull * 2ull * 5ull;

    constexpr auto TEST_TARGETS = std::array{
            std::pair{ 2, 5 },
            std::pair{ 7, 4 },
            std::pair{ 6, 5 },
            std::pair{ 7, 1 },
            std::pair{ 0, 2 },
            std::pair{ 6, 3 },
            std::pair{ 3, 1 },
            std::pair{ 4, 0 },
    };
}

void process_test() {
    auto test_state = std::array{
            std::vector{ 96ull, 60ull, 68ull, 91ull, 83ull, 57ull, 85ull, },
            std::vector{ 75ull, 78ull, 68ull, 81ull, 73ull, 99ull, },
            std::vector{ 69ull, 86ull, 67ull, 55ull, 96ull, 69ull, 94ull, 85ull, },
            std::vector{ 88ull, 75ull, 74ull, 98ull, 80ull, },
            std::vector{ 82ull, },
            std::vector{ 72ull, 92ull, 92ull, },
            std::vector{ 74ull, 61ull, },
            std::vector{ 76ull, 86ull, 83ull, 55ull, },
    };
    auto test_inspections = std::array{ 0ull, 0ull, 0ull, 0ull, 0ull, 0ull, 0ull, 0ull, };
    for (auto rounds = 0u; rounds < NUM_ROUNDS; ++rounds) {
        for (auto monkey = 0u; monkey < TEST_MONKEYS; ++monkey) {
            auto& items = test_state[monkey];
            for (auto& item : items) {
                item = TEST_OPS[monkey](item) % TEST_MODULUS;
                if (item % TEST_DIVISORS[monkey] == 0) {
                    test_state[TEST_TARGETS[monkey].first].push_back(item);
                } else {
                    test_state[TEST_TARGETS[monkey].second].push_back(item);
                }
            }
            test_inspections[monkey] += items.size();
            items.clear();
        }
    }
    ranges::sort(test_inspections);
    std::cout << test_inspections[TEST_MONKEYS - 2] * test_inspections[TEST_MONKEYS - 1] << std::endl;
}

void process_diagnostic() {
    auto diagnostic_state = std::array{
            std::vector{ 79ull, 98ull },
            std::vector{ 54ull, 65ull, 75ull, 74ull },
            std::vector{ 79ull, 60ull, 97ull },
            std::vector{ 74ull },
    };
    auto diagnostic_inspections = std::array{ 0ull, 0ull, 0ull, 0ull };
    for (auto rounds = 0u; rounds < NUM_ROUNDS; ++rounds) {
        for (auto monkey = 0u; monkey < DIAGNOSTIC_MONKEYS; ++monkey) {
            auto& items = diagnostic_state[monkey];
            for (auto& item : items) {
                item = DIAGNOSTIC_OPS[monkey](item) % DIAGNOSTIC_MODULUS;
                if (item % DIAGNOSTIC_DIVISORS[monkey] == 0) {
                    diagnostic_state[DIAGNOSTIC_TARGETS[monkey].first].push_back(item);
                } else {
                    diagnostic_state[DIAGNOSTIC_TARGETS[monkey].second].push_back(item);
                }
            }
            diagnostic_inspections[monkey] += items.size();
            items.clear();
        }
    }
    ranges::sort(diagnostic_inspections);
    std::cout << diagnostic_inspections[DIAGNOSTIC_MONKEYS - 2] * diagnostic_inspections[DIAGNOSTIC_MONKEYS - 1] << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc >= 2 && std::string{argv[1]}.ends_with("input.txt")) {
        process_test();
    } else {
        process_diagnostic();
    }
    return 0;
}
