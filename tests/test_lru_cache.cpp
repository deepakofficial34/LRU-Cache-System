/*
 * Test suite for LRU Cache
 * Run: build test_lru_cache and execute. Exit code 0 = all pass.
 */
#include "../include/LRU_Cache.hpp"
#include <cassert>
#include <iostream>
#include <string>

#define ASSERT_EQ(a, b) do { \
    if ((a) != (b)) { \
        std::cerr << "FAIL: " << #a << " == " << #b << " (got " << (a) << ", expected " << (b) << ")\n"; \
        return false; \
    } \
} while(0)

#define RUN_TEST(name) do { \
    std::cout << "  " << #name << " ... "; \
    if (name()) { std::cout << "OK\n"; passed++; } \
    else { std::cout << "FAIL\n"; failed++; } \
} while(0)

static int passed = 0, failed = 0;

// -----------------------------------------------------------------------------
// Test cases
// -----------------------------------------------------------------------------

bool test_capacity_and_size() {
    LRU_Cache c(3);
    ASSERT_EQ(c.size(), 3);
    ASSERT_EQ(c.gettin(1), "0");
    return true;
}

bool test_single_insert_and_get() {
    LRU_Cache c(2);
    c.feedin(1, "one");
    ASSERT_EQ(c.gettin(1), "one");
    ASSERT_EQ(c.gettin(2), "0");
    return true;
}

bool test_eviction_when_full() {
    LRU_Cache c(2);
    c.feedin(1, "first");
    c.feedin(2, "second");
    c.feedin(3, "third");  // should evict key 1
    ASSERT_EQ(c.gettin(1), "0");
    ASSERT_EQ(c.gettin(2), "second");
    ASSERT_EQ(c.gettin(3), "third");
    return true;
}

bool test_update_existing_key() {
    LRU_Cache c(2);
    c.feedin(1, "old");
    c.feedin(1, "new");
    ASSERT_EQ(c.gettin(1), "new");
    c.feedin(2, "two");    // order now: [two, new] -> LRU is key 1
    c.feedin(3, "three");  // evict key 1 (LRU), keep 2 and 3
    ASSERT_EQ(c.gettin(1), "0");
    ASSERT_EQ(c.gettin(2), "two");
    ASSERT_EQ(c.gettin(3), "three");
    return true;
}

bool test_mru_order_after_get() {
    // gettin() does NOT move key to MRU; eviction is by last write order only
    LRU_Cache c(2);
    c.feedin(1, "a");
    c.feedin(2, "b");      // order: [b, a] -> LRU is key 1
    (void)c.gettin(1);     // access 1 does not change list order
    c.feedin(3, "c");      // evict key 1 (still LRU), not 2
    ASSERT_EQ(c.gettin(1), "0");
    ASSERT_EQ(c.gettin(2), "b");
    ASSERT_EQ(c.gettin(3), "c");
    return true;
}

bool test_multiple_evictions() {
    LRU_Cache c(2);
    c.feedin(1, "x");
    c.feedin(2, "y");
    c.feedin(3, "z");
    c.feedin(4, "w");
    ASSERT_EQ(c.gettin(1), "0");
    ASSERT_EQ(c.gettin(2), "0");
    ASSERT_EQ(c.gettin(3), "z");
    ASSERT_EQ(c.gettin(4), "w");
    return true;
}

bool test_empty_key_lookup() {
    LRU_Cache c(5);
    ASSERT_EQ(c.gettin(0), "0");
    ASSERT_EQ(c.gettin(100), "0");
    return true;
}

bool test_capacity_one() {
    LRU_Cache c(1);
    c.feedin(1, "only");
    ASSERT_EQ(c.gettin(1), "only");
    c.feedin(2, "replaces");
    ASSERT_EQ(c.gettin(1), "0");
    ASSERT_EQ(c.gettin(2), "replaces");
    return true;
}

bool test_string_content() {
    LRU_Cache c(2);
    c.feedin(1, "hello world");
    c.feedin(2, "123");
    ASSERT_EQ(c.gettin(1), "hello world");
    ASSERT_EQ(c.gettin(2), "123");
    return true;
}

bool test_demo_flow() {
    // Matches original main() behaviour
    LRU_Cache lru1(2);
    if (lru1.size() != 2) return false;
    lru1.feedin(1, "beta");
    lru1.feedin(3, "alpha");
    lru1.feedin(8, "gamma");
    if (lru1.gettin(1) != "0") return false;
    if (lru1.gettin(3) != "alpha") return false;
    if (lru1.gettin(6) != "0") return false;
    if (lru1.gettin(8) != "gamma") return false;
    return true;
}

int main() {
    std::cout << "LRU Cache test suite\n";
    std::cout << "--------------------\n";

    RUN_TEST(test_capacity_and_size);
    RUN_TEST(test_single_insert_and_get);
    RUN_TEST(test_eviction_when_full);
    RUN_TEST(test_update_existing_key);
    RUN_TEST(test_mru_order_after_get);
    RUN_TEST(test_multiple_evictions);
    RUN_TEST(test_empty_key_lookup);
    RUN_TEST(test_capacity_one);
    RUN_TEST(test_string_content);
    RUN_TEST(test_demo_flow);

    std::cout << "--------------------\n";
    std::cout << "Passed: " << passed << "  Failed: " << failed << "\n";
    return failed == 0 ? 0 : 1;
}
