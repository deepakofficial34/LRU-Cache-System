# Complete Project Explanation — LRU Cache

This file explains **everything** about this project in detail. No prior knowledge of caches or data structures is assumed.

---

## Table of Contents

1. [What is this project?](#1-what-is-this-project)
2. [What is a cache?](#2-what-is-a-cache)
3. [What does “LRU” mean?](#3-what-does-lru-mean)
4. [Why is LRU useful in real life?](#4-why-is-lru-useful-in-real-life)
5. [What does this project store? (keys and values)](#5-what-does-this-project-store-keys-and-values)
6. [How does the LRU cache work? (big picture)](#6-how-does-the-lru-cache-work-big-picture)
7. [How is it implemented? (data structures)](#7-how-is-it-implemented-data-structures)
8. [Walking through the code](#8-walking-through-the-code)
9. [What each method does (in detail)](#9-what-each-method-does-in-detail)
10. [Important detail: “recently used” = last write, not last read](#10-important-detail-recently-used--last-write-not-last-read)
11. [Project files and what they do](#11-project-files-and-what-they-do)
12. [How to build, run, and test](#12-how-to-build-run-and-test)
13. [What each test case checks](#13-what-each-test-case-checks)
14. [Quick reference and glossary](#14-quick-reference-and-glossary)

---

## 1. What is this project?

This project is a **C++ implementation of an LRU (Least Recently Used) cache**.  
In simple terms: it is a **fixed-size box** that stores **key–value pairs** (here: integer key → string value). When the box is full and you add something new, the item that was **least recently used** is removed to make room. The code uses a **doubly linked list** and an **unordered map** to do this efficiently.

- **Author:** Aditya Paneru (IIIT Lucknow)  
- **Topics:** Data Structures & Algorithms (DSA), Object-Oriented Programming (OOP), Operating Systems (OS)

---

## 2. What is a cache?

A **cache** is a small, fast storage that keeps a **copy** of some data so you don’t have to fetch it from a slow place every time.

- **Slow storage:** e.g. hard disk, database, another server — takes more time to read.
- **Cache:** e.g. RAM or a small in-memory structure — much faster to read.

**Idea:** Keep the “hot” (frequently or recently used) data in the cache. When you need something, first check the cache; if it’s there (a **cache hit**), use it. If not (a **cache miss**), get it from the slow storage and optionally put a copy in the cache for next time.

So a cache **speeds things up** by avoiding slow reads when the same data is needed again.

---

## 3. What does “LRU” mean?

**LRU = Least Recently Used.**

It is a **rule for eviction** (throwing something out when the cache is full):

- When the cache is **full** and you want to add a **new** item, you must remove one old item.
- **LRU says:** remove the one that was **least recently used** (the “oldest” in terms of use).
- The ones you used **more recently** stay in the cache.

So:

- **Recently used** → keep (likely to be used again).
- **Not used for a long time** → remove first when space is needed.

In this project, “recently used” means **recently written (inserted or updated)**, not “recently read.” Reading a value with `gettin()` does **not** change which item is considered “least recently used.” (See [Section 10](#10-important-detail-recently-used--last-write-not-last-read).)

---

## 4. Why is LRU useful in real life?

Two common uses:

### A. Backend servers / websites

- A server has lots of data (e.g. pages, user data) on disk or in a database — slow to read.
- It keeps a **small cache in memory** (e.g. LRU) of recently requested items.
- When a user asks for a page that is already in the cache, the server answers from memory (fast). When the cache is full, it evicts the least recently used item.
- Result: faster responses and less load on the slow storage.

### B. Operating systems — page replacement

- The CPU uses **RAM** (fast) and **disk** (slow). Memory is split into “pages.”
- Not all pages of a program can be in RAM at once. When the CPU needs a page that is not in RAM, that’s a **page fault** — the OS must bring that page from disk (slow).
- **Page replacement:** when RAM is full and a new page must be brought in, the OS must choose which current page to throw out. One popular policy is **LRU**: evict the least recently used page.
- This project implements the **data structure** that supports such an LRU policy (fixed capacity, add, get, and evict least recently used).

---

## 5. What does this project store? (keys and values)

- **Key:** an **integer** (e.g. 1, 3, 8). Think of it like a page number or an ID.
- **Value:** a **string** (e.g. `"alpha"`, `"hello world"`). Think of it as the content (e.g. text of a page).

So the cache stores **integer → string** pairs.  
Example: key `3` might map to the string `"alpha"`. That way you can store both numbers and text (as strings), and use integers as simple IDs for fast lookup.

---

## 6. How does the LRU cache work? (big picture)

- You create a cache with a **capacity** (e.g. 2 or 10). That is the **maximum number of key–value pairs** the cache can hold.
- You can:
  - **Put** a key–value pair: `feedin(key, value)`. If the key is new and the cache is full, the **least recently used** entry is removed, then the new one is added. If the key already exists, its value is updated and it becomes “most recently used.”
  - **Get** the value for a key: `gettin(key)`. If the key is in the cache, you get its string; if not, you get `"0"` (meaning “not in cache”).
- **Order of “recently used”:** the implementation keeps an internal order. The **most recently used** (MRU) item is at the **front**, and the **least recently used** (LRU) item is at the **back**. When the cache is full and you add a new item, the one at the **back** (LRU) is evicted.

**Example (capacity = 2):**

1. `feedin(1, "beta")`   → Cache: `[1 → "beta"]`
2. `feedin(3, "alpha")`   → Cache: `[3 → "alpha", 1 → "beta"]` (newest first)
3. `feedin(8, "gamma")`  → Cache is full. Evict LRU (key 1). Cache: `[8 → "gamma", 3 → "alpha"]`
4. `gettin(1)` → `"0"` (key 1 was evicted)
5. `gettin(3)` → `"alpha"`
6. `gettin(8)` → `"gamma"`

---

## 7. How is it implemented? (data structures)

The implementation uses two things:

### A. Doubly linked list — `L`

- Stores the **values** (strings) in **order of use**.
- **Front** = most recently used (MRU).
- **Back** = least recently used (LRU).
- We can quickly: add at front, remove from back, or remove a node in the middle (when we update an existing key). So we can keep the “usage order” correct.

### B. Unordered map — `M`

- **Key:** same integer key as in the cache (e.g. 1, 3, 8).
- **Value:** a **pointer (iterator)** into the list `L` that points to the node where that key’s string is stored.
- So: given a key, we can **instantly** find its node in the list (and thus its string), and we can **remove that node** or move the key to the front by list operations.

**Why both?**

- **List alone:** finding “which key is at the back?” or “where is key 5?” would be slow (we’d walk the list).
- **Map alone:** we don’t have a clear “order” of use; we’d need something else to know who is LRU.
- **List + map together:** the list gives **order** (front = MRU, back = LRU); the map gives **O(1) lookup by key** and **O(1) access to the right list node**. So both “get by key” and “evict LRU” can be done in constant time.

**Summary:**

| Part        | Type        | Role                                      |
|------------|-------------|-------------------------------------------|
| `L`        | list of strings | Order: front = MRU, back = LRU         |
| `M`        | map: key → iterator in `L` | Fast lookup and fast update of list |
| `capacity` | int         | Max number of entries in the cache        |

---

## 8. Walking through the code

### The class (from `include/LRU_Cache.hpp`)

```cpp
class LRU_Cache {
public:
    LRU_Cache(int cap);                      // Constructor: create cache with capacity cap
    int size() const;                        // Return capacity
    void feedin(int key, const std::string& data);  // Insert or update (key, data)
    std::string gettin(int key) const;       // Get value for key, or "0" if missing
private:
    std::list<std::string> L;                // List: front = MRU, back = LRU
    std::unordered_map<int, std::list<std::string>::iterator> M;  // key → iterator in L
    int capacity;
};
```

- **L:** holds the strings in “recently used” order.
- **M:** for each key, stores where that key’s string lives in **L**.
- **capacity:** max size of the cache.

### What the demo does (`src/main.cpp`)

1. Create a cache of capacity **2**: `LRU_Cache lru1(2);`
2. Print capacity: `lru1.size()` → `2`
3. `feedin(1, "beta")`   → cache has one entry: key 1.
4. `feedin(3, "alpha")`  → cache has two entries: keys 1 and 3. (Full.)
5. `feedin(8, "gamma")`  → cache is full, so key 1 (LRU) is evicted; now cache has keys 3 and 8.
6. `gettin(1)` → key 1 not in cache → `"0"`
7. `gettin(3)` → `"alpha"`
8. `gettin(6)` → key 6 never added → `"0"`
9. `gettin(8)` → `"gamma"`

So the demo shows: capacity, insert, eviction when full, and get (hit and miss).

---

## 9. What each method does (in detail)

### Constructor: `LRU_Cache(int cap)`

- If `cap > 0`: sets `capacity = cap`, and clears the list and the map (empty cache).
- If `cap <= 0`: prints an error and sets `capacity = 0` (cache is effectively disabled). This avoids undefined behaviour.

### `int size() const`

- Returns the **capacity** (the maximum number of entries), not the current number of entries.

### `void feedin(int key, const std::string& data)`

- **If the key is not in the cache:**
  - If the cache is **full** (`L.size() == capacity`): find the key that points to the **last** element of `L` (that key is LRU), remove that key from `M`, and remove that last element from `L` (eviction).
  - Then: add `data` at the **front** of `L`, and set `M[key] = L.begin()` (so the key now points to this new front node).
- **If the key is already in the cache:**
  - Remove the old node for this key from `L` (using the iterator stored in `M[key]`).
  - Add `data` at the **front** of `L`, and set `M[key] = L.begin()`.

So every new or updated key ends up at the **front** (MRU). The **back** of the list is always the LRU and is evicted when the cache is full.

### `std::string gettin(int key) const`

- Look up `key` in `M`.
- If not found: return `"0"` (meaning “not in cache”).
- If found: return the string at the iterator `M[key]` (i.e. the value stored in the list for that key).

Note: `gettin` does **not** change the list or the map. So “recently used” is **not** updated when you only read; only `feedin` updates the order. (See next section.)

---

## 10. Important detail: “recently used” = last write, not last read

In this implementation, **“recently used” means “recently inserted or updated”** (i.e. last **write**), not “recently read.”

- **feedin(key, value)** → that key becomes **most recently used** (moved to front).
- **gettin(key)** → you only **read** the value; the key’s position in the list **does not change**.

So if you have capacity 2, add keys 1 and 2, then only **read** key 1 many times, key 1 is **still** the LRU (because it wasn’t written again). When you add key 3, key 1 will be evicted.

If you wanted “LRU by last **access**” (read or write), you would need to move the node to the front inside `gettin()` when the key is found. This project deliberately does not do that, so eviction is purely by **last write order**.

---

## 11. Project files and what they do

| File / folder | Purpose |
|---------------|--------|
| **include/LRU_Cache.hpp** | The LRU cache class and its implementation (header-only). This is the main “logic” of the project. |
| **src/main.cpp** | Demo program: creates a cache, calls `feedin` and `gettin`, prints results. |
| **tests/test_lru_cache.cpp** | Test suite: 10 tests that check capacity, insert, get, eviction, update, edge cases. Run this to verify the code works. |
| **LRU Cache Project File.cpp** | Original single-file version with detailed comments (legacy). |
| **Makefile** | Builds the demo and tests on Linux/macOS (`make all`, `make run`, `make run-test`). |
| **build.ps1** | Builds demo and tests on Windows (PowerShell) and runs the tests. |
| **.gitignore** | Tells Git to ignore build outputs (e.g. `.exe`, `.o`). |
| **.github/workflows/build-and-test.yml** | GitHub Actions: on every push, builds and runs tests so the repo can show “build passing.” |
| **README.md** | Overview, build/run instructions, API, motivation. |
| **EXPLANATION.md** | This file — full explanation of the project. |

---

## 12. How to build, run, and test

### Build and run the demo

**Windows (PowerShell):**

```powershell
g++ -std=c++17 -Wall -I. -o lru_cache.exe src/main.cpp
.\lru_cache.exe
```

**Linux/macOS:**

```bash
make all
./lru_cache
# or: g++ -std=c++17 -Wall -I. -o lru_cache src/main.cpp && ./lru_cache
```

### Build and run the tests

**Windows:**

```powershell
g++ -std=c++17 -Wall -I. -o test_lru_cache.exe tests/test_lru_cache.cpp
.\test_lru_cache.exe
```

**Linux/macOS:**

```bash
make all
./test_lru_cache
```

**One command on Windows (build + test):**

```powershell
.\build.ps1
```

If everything works, you should see **Passed: 10  Failed: 0** and the program exits with code 0.

---

## 13. What each test case checks

| Test name | What it checks |
|-----------|----------------|
| **test_capacity_and_size** | After creating a cache with capacity 3, `size()` returns 3, and `gettin(1)` returns `"0"` (empty cache). |
| **test_single_insert_and_get** | Insert one key–value; get returns that value; get for another key returns `"0"`. |
| **test_eviction_when_full** | Capacity 2; add keys 1, 2, then 3. Key 1 is evicted: gettin(1)=`"0"`, gettin(2) and gettin(3) return their values. |
| **test_update_existing_key** | Update key 1 from `"old"` to `"new"`; gettin(1)=`"new"`. Then add key 2, then key 3; key 1 is evicted (order is by last write). |
| **test_mru_order_after_get** | Confirms that **reading** with `gettin(1)` does **not** move key 1 to MRU; when we add key 3, key 1 (still LRU) is evicted. |
| **test_multiple_evictions** | Capacity 2; add 1,2,3,4. Only 3 and 4 remain; 1 and 2 are evicted. |
| **test_empty_key_lookup** | For an empty cache, gettin(0) and gettin(100) both return `"0"`. |
| **test_capacity_one** | Capacity 1; add key 1, then key 2. Key 1 is evicted; gettin(1)=`"0"`, gettin(2) returns the value for 2. |
| **test_string_content** | Values can be any string (e.g. `"hello world"`, `"123"`); get returns them correctly. |
| **test_demo_flow** | Repeats the exact sequence from `main.cpp` and checks all outputs (size, gettin(1), gettin(3), gettin(6), gettin(8)). |

Together, these tests show that the cache behaves correctly for capacity, insert, get, eviction, update, and the “no promotion on get” behaviour.

---

## 14. Quick reference and glossary

- **Cache:** Fast storage that holds a copy of some data to avoid slow access (e.g. to disk or network).
- **LRU (Least Recently Used):** Eviction rule: when the cache is full, remove the item that was used least recently (here: least recently **written**).
- **MRU (Most Recently Used):** The item that was used most recently (here: last inserted or updated).
- **Eviction:** Removing an item from the cache to make room for a new one.
- **Capacity:** Maximum number of key–value pairs the cache can hold.
- **feedin(key, data):** Insert or update; makes this key the MRU. If full, evicts the LRU first.
- **gettin(key):** Return the value for `key`, or `"0"` if not in cache. Does not change “recently used” order.
- **Hit / Miss:** A “hit” is when the key is in the cache; a “miss” is when it is not (gettin returns `"0"`).

---

*This document explains the LRU Cache project in detail. For a shorter overview and build instructions, see **README.md**.*
