/*
 * Demo: LRU Cache
 * Build and run to see basic usage and output.
 */
#include "../include/LRU_Cache.hpp"
#include <iostream>

int main() {
    LRU_Cache lru1(2);

    std::cout << "The size of this LRU Cache is : " << lru1.size() << "\n";

    lru1.feedin(1, "beta");
    lru1.feedin(3, "alpha");
    lru1.feedin(8, "gamma");  // evicts key 1 (capacity 2)

    std::cout << lru1.gettin(1) << "\n";   // evicted -> "0"
    std::cout << lru1.gettin(3) << "\n";   // "alpha"
    std::cout << lru1.gettin(6) << "\n";   // never inserted -> "0"
    std::cout << lru1.gettin(8) << "\n";   // "gamma"

    return 0;
}
