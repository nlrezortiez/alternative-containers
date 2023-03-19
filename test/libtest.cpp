#include "../lib/rzt_stack.hpp"
#include "../lib/rzt_vector.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>

#include <gtest/gtest.h>

template <typename T>
void sandbox() {
    
    std::string string = "jopa";
    std::string string1 = "jopa2";
    rzt::vector<T> v;
    for (int i = 0; i < 6; ++i) {
        v.push_back(string);
        std::cout << v.capacity() << " " << v.size() << std::endl;
    }
    std::cout << v[2];
}

int main(int argc, char *argv[]) {
    sandbox<std::string>();
    //::testing::InitGoogleTest(&argc, argv);
    //return RUN_ALL_TESTS();
    return 0;
}
