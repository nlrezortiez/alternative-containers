#include "../lib/rzt_stack.hpp"
#include "../lib/rzt_vector.hpp"

#include <vector>
#include <iostream>
#include <iterator>

#include <gtest/gtest.h>

template <typename T>
void sandbox() {
    static_assert(std::random_access_iterator<typename rzt::vector<T>::iterator>, "doesn't satisfy rat policy");
}

int main(int argc, char *argv[]) {
    sandbox<std::string>();
    //::testing::InitGoogleTest(&argc, argv);
    //return RUN_ALL_TESTS();
    return 0;
}
