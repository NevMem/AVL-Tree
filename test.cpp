#include <iostream>
#include <algorithm>
#include <vector>
#include <stdexcept>
#include "avl.cpp"

int main() {
    {
        try {
            Set<int> a;
            for (size_t i = 0; i != 1000000; ++i) {
                a.insert(i);
            }
            std::cout << a.size() << "\n";
        } catch (const std::exception& e) {
            std::cout << "Exception was caught\n";
            std::cout << e.what() << "\n";
        }
    }
    std::cout << "constructed: " << NevMem::constructed << "\n";
    std::cout << "deconstructed: " << NevMem::deconstructed << "\n";
}