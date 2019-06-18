#include <iostream>
#include <algorithm>
#include <vector>
#include "avl.cpp"

int main() {
    Set<int> a;
    for (size_t i = 0; i != 1000; ++i) {
        a.insert(i);
    }
    std::cout << a.size() << "\n";
    std::vector<int> b;
    for (auto elem : a) {
        b.push_back(elem);
    }
    std::cout << a.size() << " " << b.size() << "\n";
    for (auto elem : b) {
        std::cout << elem << "\n";
    }
}