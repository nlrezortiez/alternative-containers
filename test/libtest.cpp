#include "../lib/rzt_stack.hpp"
#include "../lib/rzt_vector.hpp"

#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>

#include <gtest/gtest.h>

struct simple_struct {
    simple_struct() = default;
    simple_struct(int a, double b, std::string c) : a(a), b(b), c(c) {}

    int a;
    double b;
    std::string c;
};

std::ostream& operator<<(std::ostream& out, simple_struct item) {
    out << item.a << " " << item.b << " " << item.c;
    return out;
}

template <typename T>
void sandbox(rzt::vector<std::string>& pvector) {
    rzt::vector<simple_struct> struct_vec;
    struct_vec.emplace_back(1, 2.3, "qwe");
    std::cout << struct_vec[0];
}

int main(int argc, char* argv[]) {
    rzt::vector<std::string> vec(10, "wtf");
    sandbox<std::string>(vec);
    return 0;
}
