#include "../lib/rzt_stack.hpp"
#include "../lib/rzt_vector.hpp"

#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>

#include <gtest/gtest.h>

template <typename T>
void sandbox() {
    rzt::vector<T> v1;
    v1.push_back("index0");
    v1.push_back("index1");
    v1.push_back("index2");
    v1.push_back("index3");
    v1.insert(v1.begin() + 2, "inserted");
    for (const auto& string : v1) {
        std::cout << string << std::endl;
    }
}

int main(int argc, char *argv[]) {
    sandbox<std::string>();
    //::testing::InitGoogleTest(&argc, argv);
    //return RUN_ALL_TESTS();
    return 0;
}
