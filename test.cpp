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
            Set<int> b = a;
            std::cout << a.size() << " " << b.size() << "\n";
        } catch (const std::exception& e) {
            std::cerr << "Exception was caught\n";
            std::cerr << e.what() << "\n";
        }
    }
    {
        try {
            std::vector<int> a;
            for (size_t i = 0; i != 1000000; ++i)
                a.push_back(i);
            std::cout << a.size() << "\n";
            Set<int> b {a.begin(), a.end()};
            std::cout << b.size() << "\n";
        } catch (const std::exception& e) {
            std::cerr << "Another exception\n";
            std::cerr << e.what() << "\n";
        }
    }
    std::cout << "constructed: " << NevMem::constructed << "\n";
    std::cout << "deconstructed: " << NevMem::deconstructed << "\n";
}