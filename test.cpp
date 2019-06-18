#include <iostream>
#include <algorithm>
#include <vector>
#include "avl.cpp"

int main() {
    {
        Set<int> a;
        for (size_t i = 0; i != 1000000; ++i) {
            a.insert(i);
        }
        std::cout << a.size() << "\n";
    }
    std::cout << "constructed: " << NevMem::constructed << "\n";
    std::cout << "deconstructed: " << NevMem::deconstructed << "\n";
}