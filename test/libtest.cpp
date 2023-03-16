#include "../lib/rzt_stack.hpp"
#include "../lib/rzt_vector.hpp"

#include <vector>
#include <iostream>
#include <iterator>

#include <gtest/gtest.h>

template <bool IsConst, typename T, typename F>
struct conditional {
    using type = F;
};

template <typename T, typename F>
struct conditional<true, T, F> {
    using type = T;
};

template <bool IsConst, typename T, typename F>
using conditional_t = typename conditional<IsConst, T, F>::type;

void sandbox() {
    std::vector<int> v;
}

using namespace rzt;

TEST(StackTestSuite, StackCopyEqualityTest) {
    stack<int> test_stack;
    for (int i = 0; i < 5; ++i) {
        test_stack.push(i);
    }
    stack<int> test_stack_2(test_stack);
    auto it1 = test_stack.begin();
    auto it2 = test_stack_2.begin();
    for (;it1 != test_stack.end() || it2 != test_stack_2.end(); ++it1, ++it2) {
        ASSERT_EQ(*it1, *it2);
    }
}

int main(int argc, char *argv[]) {
    std::vector
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}