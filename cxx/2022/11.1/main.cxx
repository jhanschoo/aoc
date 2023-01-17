#include <bits/stdc++.h>
#include <range/v3/all.hpp>

namespace {
    using lli = long long;

    constexpr auto DIAGNOSTIC_MONKEYS = 4u;

    constexpr auto NUM_ROUNDS = 20;

    constexpr auto DIAGNOSTIC_OPS = std::array{
            +[](lli old) { return old * 19; },
            +[](lli old) { return old + 6; },
            +[](lli old) { return old * old; },
            +[](lli old) { return old + 3; },
    };

    constexpr auto DIAGNOSTIC_DIVISORS = std::array{ 23LL, 19LL, 13LL, 17LL };

    constexpr auto DIAGNOSTIC_TARGETS = std::array{
            std::pair{ 2, 3 },
            std::pair{ 2, 0 },
            std::pair{ 1, 3 },
            std::pair{ 0, 1 },
    };

    const auto TEST_MONKEYS = 8u;

    constexpr auto TEST_OPS = std::array{
            +[](lli old) { return old * 2; },
            +[](lli old) { return old + 3; },
            +[](lli old) { return old + 6; },
            +[](lli old) { return old + 5; },
            +[](lli old) { return old + 8; },
            +[](lli old) { return old * 5; },
            +[](lli old) { return old * old; },
            +[](lli old) { return old + 4; },
    };

    constexpr auto TEST_DIVISORS = std::array{ 17LL, 13LL, 19LL, 7LL, 11LL, 3LL, 2LL, 5LL };

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
            std::vector{ 96LL, 60LL, 68LL, 91LL, 83LL, 57LL, 85LL, },
            std::vector{ 75LL, 78LL, 68LL, 81LL, 73LL, 99LL, },
            std::vector{ 69LL, 86LL, 67LL, 55LL, 96LL, 69LL, 94LL, 85LL, },
            std::vector{ 88LL, 75LL, 74LL, 98LL, 80LL, },
            std::vector{ 82LL, },
            std::vector{ 72LL, 92LL, 92LL, },
            std::vector{ 74LL, 61LL, },
            std::vector{ 76LL, 86LL, 83LL, 55LL, },
    };
    auto test_inspections = std::array{ 0ull, 0ull, 0ull, 0ull, 0ull, 0ull, 0ull, 0ull, };
    for (auto rounds = 0; rounds < NUM_ROUNDS; ++rounds) {
        for (auto monkey = 0u; monkey < TEST_MONKEYS; ++monkey) {
            auto& items = test_state[monkey];
            for (auto& item : items) {
                item = TEST_OPS[monkey](item);
                item /= 3;
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
//    for (auto &items : test_state) {
//        for (auto item : items) {
//            std::cout << item << ' ';
//        }
//        std::cout << std::endl;
//    }
}

void process_diagnostic() {
    auto diagnostic_state = std::array{
            std::vector{ 79LL, 98LL },
            std::vector{ 54LL, 65LL, 75LL, 74LL },
            std::vector{ 79LL, 60LL, 97LL },
            std::vector{ 74LL },
    };
    auto diagnostic_inspections = std::array{ 0ull, 0ull, 0ull, 0ull };
    for (auto rounds = 0; rounds < NUM_ROUNDS; ++rounds) {
        for (auto monkey = 0u; monkey < DIAGNOSTIC_MONKEYS; ++monkey) {
            auto& items = diagnostic_state[monkey];
            for (auto& item : items) {
                item = DIAGNOSTIC_OPS[monkey](item);
                item /= 3;
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
//    for (auto &items : diagnostic_state) {
//        for (auto item : items) {
//            std::cout << item << ' ';
//        }
//        std::cout << std::endl;
//    }
}

int main(int argc, char* argv[]) {
    if (argc >= 2 && std::string{argv[1]}.ends_with("input.txt")) {
        process_test();
    } else {
        process_diagnostic();
    }
    return 0;
}
