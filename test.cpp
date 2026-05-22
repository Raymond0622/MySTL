#include <iostream>
#include <chrono>
#include <unordered_map>
#include <vector>
#include <random>
#include <algorithm>
#include <map>


__attribute__((noinline)) void stuff(int x) {
    long long curr = 0;
    for (int i = 0; i < 1000;i++) {
        curr += x;
    }
    std::cout << curr << "\n";
    return;
}

inline __attribute__((always_inline)) void stuff1(int x) {
    long long curr = 0;
    for (int i = 0; i < 1000;i++) {
        curr += x;
    }
    std::cout << curr << "\n";
    return;
}

int main() {
    int x = 10000000;
    auto start = std::chrono::high_resolution_clock::now();
    while (x > 0) {
        x--;
        if (x == 0) {
            stuff(x);
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    double c = std::chrono::duration<double>(end - start).count();
    std::cout << c;
}