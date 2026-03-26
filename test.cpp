#include <iostream>
#include <array>

struct A {
    int t[10];
};


int main() {
    std::array<int, 20> v;
    A a;
    std::cout << sizeof(v);
}