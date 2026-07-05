/*
 * LRU (Least Recently Used) Cache
 * Implementation using doubly linked list + unordered map.
 * Author: Aditya Paneru | IIIT Lucknow
 */
#ifndef LRU_CACHE_HPP
#define LRU_CACHE_HPP

#include <iostream>
#include <list>
#include <unordered_map>
#include <string>

class LRU_Cache {
public:
    LRU_Cache(int cap);
    int size() const;
    void feedin(int key, const std::string& data);
    std::string gettin(int key) const;

private:
    std::list<std::string> L;  // front = MRU, back = LRU
    std::unordered_map<int, std::list<std::string>::iterator> M;
    int capacity;
};

// -----------------------------------------------------------------------------
// Implementation
// -----------------------------------------------------------------------------

inline LRU_Cache::LRU_Cache(int cap) : capacity(0) {
    if (cap > 0) {
        capacity = cap;
    } else {
        std::cout << "ERROR: LRU Capacity must be more than 0. Please reinitialise the cache.\n";
        std::cout << "Random behaviour may occur since capacity is set to 0.\n";
    }
    L.clear();
    M.clear();
}

inline int LRU_Cache::size() const {
    return capacity;
}

inline void LRU_Cache::feedin(int key, const std::string& data) {
    if (M.find(key) == M.end()) {
        if (capacity > 0 && L.size() == static_cast<size_t>(capacity)) {
            for (auto it = M.begin(); it != M.end(); ++it) {
                if (it->second == --L.end()) {
                    M.erase(it);
                    break;
                }
            }
            L.pop_back();
        }
        L.push_front(data);
        M[key] = L.begin();
        return;
    }
    L.erase(M[key]);
    L.push_front(data);
    M[key] = L.begin();
}

inline std::string LRU_Cache::gettin(int key) const {
    auto it = M.find(key);
    if (it == M.end())
        return "0";
    return *it->second;
}

#endif // LRU_CACHE_HPP
