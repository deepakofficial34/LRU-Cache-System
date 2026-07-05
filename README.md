# LRU Cache Implementation

An **LRU (Least Recently Used)** cache implementation in C++ using a **doubly linked list** and an **unordered map**. The cache stores **strings** with **integer keys**, suitable for scenarios like page caching (e.g., page number → page content).

- **Author:** Aditya Paneru 
- **Relevant areas:** Data Structures & Algorithms (DSA), Object-Oriented Programming (OOP), Operating Systems (OS)  
- **Language:** C++  

---

## Table of Contents

- [Full project explanation](#full-project-explanation) → **EXPLANATION.md**
- [Project Structure](#project-structure)
- [What is an LRU Cache?](#what-is-an-lru-cache)
- [Project Flow (How It Works)](#project-flow-how-it-works)
- [Data Structures Used](#data-structures-used)
- [API Reference](#api-reference)
- [Build and Run](#build-and-run)
- [Running Tests](#running-tests)
- [Is the code working?](#is-the-code-working)
- [Example Run and Output](#example-run-and-output)
- [Motivation and Use Cases](#motivation-and-use-cases)
- [LRU in Operating Systems](#lru-in-operating-systems)
- [Code Overview](#code-overview)

---

## Full project explanation

**New to the project?** Read **[EXPLANATION.md](EXPLANATION.md)** for a step-by-step explanation of everything: what a cache is, what LRU means, how the code works, what each file and test does, and how to build and run. No prior background assumed.

---

## Project Structure

```
LRU-Cache-Implementation/
├── include/
│   └── LRU_Cache.hpp      # LRU cache class (header-only)
├── src/
│   └── main.cpp           # Demo program
├── tests/
│   └── test_lru_cache.cpp # Test suite
├── Makefile               # Build on Linux/macOS
├── build.ps1              # Build + run tests on Windows (PowerShell)
├── .gitignore
├── README.md
├── EXPLANATION.md               # Detailed explanation of the whole project
└── LRU Cache Project File.cpp  # Original single-file version (legacy)
```

---

## What is an LRU Cache?

In an LRU cache:

- **Recently used** items are kept “at the front” (fast to access).
- When the cache is **full**, the **least recently used** item is **evicted** to make room for the new one.

So the cache automatically keeps “hot” data and drops “cold” data, which is useful for speeding up repeated lookups (e.g., in servers or in OS page replacement).

---

## Project Flow (How It Works)

1. **Creation**  
   - You create an `LRU_Cache` with a fixed **capacity** (e.g., 2).  
   - Internally: an empty list and an empty map are ready.

2. **Insert/Update (`feedin(key, data)`)**  
   - **If the key is new:**
     - If the cache is **full**, the **last** element in the list (least recently used) is removed from both the list and the map.
     - The new `(key, data)` is added at the **front** of the list and the map stores `key → iterator` to that node.
   - **If the key already exists:**
     - The old list node for that key is removed, and the new data is inserted at the **front**.
     - The map is updated so the key points to this new front node.

3. **Lookup (`gettin(key)`)**  
   - If the key is in the map, the value at the stored iterator is returned.  
   - If not, `"0"` is returned (meaning “not in cache”).

**Order in the list:**  
- **Front** = most recently inserted/updated.  
- **Back** = least recently inserted/updated (candidate for eviction when full).

---

## Data Structures Used

| Component | Type | Role |
|----------|------|------|
| `L` | `list<string>` (doubly linked list) | Stores the cached strings. Front = MRU, back = LRU. |
| `M` | `unordered_map<int, list<string>::iterator>` | Maps each integer key to the list iterator of its value for O(1) lookup and update. |
| `capacity` | `int` | Maximum number of entries in the cache. |

**Why this design?**

- **List:** Moving a node to the front or removing the back is O(1).
- **Map:** Finding whether a key exists and getting its iterator is O(1) on average.
- Together they give **O(1)** `feedin` and `gettin` in typical use.

---

## API Reference

| Method | Description |
|--------|-------------|
| `LRU_Cache(int cap)` | Constructor. `cap` must be > 0; otherwise a warning is printed and capacity is 0. |
| `int size()` | Returns the cache capacity (not the current number of elements). |
| `void feedin(int key, string data)` | Inserts or updates `data` for `key`. On full cache, evicts the least recently used entry. |
| `string gettin(int key)` | Returns the string for `key`, or `"0"` if the key is not in the cache. |

---

## Build and Run

### Prerequisites

- A C++ compiler (e.g., **g++**, **MinGW**, or **MSVC**) with C++17 (or C++11 minimum).

### Option 1: Build scripts

**Windows (PowerShell)** — builds demo + tests and runs tests:

```powershell
cd "c:\Users\adity\projects\LRU-Cache-Implementation"
.\build.ps1
```

**Linux / macOS (Makefile):**

```bash
make all      # build demo and tests
make run      # run demo
make run-test # run tests
make clean    # remove binaries
```

### Option 2: Manual build

From the project root:

```powershell
# Demo
g++ -std=c++17 -Wall -I. -o lru_cache.exe src/main.cpp

# Tests
g++ -std=c++17 -Wall -I. -o test_lru_cache.exe tests/test_lru_cache.cpp
```

### Run demo

```powershell
.\lru_cache.exe
```

On Linux/macOS: `./lru_cache`

---

## Running Tests

The test suite checks capacity, eviction, updates, and edge cases. All tests must pass before pushing to GitHub.

**Run tests:**

```powershell
.\test_lru_cache.exe
```

On Linux/macOS: `./test_lru_cache`

**Expected output (all pass):**

```
LRU Cache test suite
--------------------
  test_capacity_and_size ... OK
  test_single_insert_and_get ... OK
  test_eviction_when_full ... OK
  test_update_existing_key ... OK
  test_mru_order_after_get ... OK
  test_multiple_evictions ... OK
  test_empty_key_lookup ... OK
  test_capacity_one ... OK
  test_string_content ... OK
  test_demo_flow ... OK
--------------------
Passed: 10  Failed: 0
```

**Test cases covered:**

| Test | Description |
|------|-------------|
| `test_capacity_and_size` | Constructor and `size()` |
| `test_single_insert_and_get` | One key-value insert and get |
| `test_eviction_when_full` | LRU eviction when capacity exceeded |
| `test_update_existing_key` | Updating a key moves it to MRU |
| `test_mru_order_after_get` | `gettin()` does not change order (eviction by write only) |
| `test_multiple_evictions` | Several evictions in sequence |
| `test_empty_key_lookup` | Missing keys return `"0"` |
| `test_capacity_one` | Cache of size 1 |
| `test_string_content` | Non-numeric string values |
| `test_demo_flow` | Matches original demo behaviour |

---

## Example Run and Output

The `main()` in the project file does the following:

1. Creates an LRU cache of **capacity 2**.
2. Prints the cache size: `2`.
3. Calls:
   - `feedin(1, "beta")`  → cache: `[beta]`
   - `feedin(3, "alpha")` → cache: `[alpha, beta]`
   - `feedin(8, "gamma")` → cache is full; **key 1 ("beta")** is evicted → cache: `[gamma, alpha]`
4. Then:
   - `gettin(1)` → key 1 was evicted → **0**
   - `gettin(3)` → **alpha**
   - `gettin(6)` → never inserted → **0**
   - `gettin(8)` → **gamma**

**Expected output:**

```
The size of this LRU Cache is : 2
0
alpha
0
gamma
```

---

## Motivation and Use Cases

- Many examples online implement LRU caches for **integers** only. Here the cache stores **strings** with **integer keys**, so you can cache text (or any string data) by a numeric id (e.g., page number).
- **Example:** A server has text stored in “pages” with page numbers. To avoid slow reads from disk, it keeps recently requested pages in an LRU cache: key = page number (int), value = page content (string). This project mirrors that idea.
- **Backend servers:** LRU is commonly used to cache responses or data so that frequently requested content is served from memory instead of secondary storage, improving latency.

---

## LRU in Operating Systems

*(Summary of the ideas behind LRU in OS; for exact terminology and details, refer to your course material.)*

- To use the CPU well, we run many processes (multiprogramming). When a process waits for I/O, the CPU can run others.
- **Memory management** (e.g., **paging**) divides processes into **pages** and memory into **frames**. Only some pages of each process are in RAM; the rest stay on disk (**virtual memory**).
- A **page fault** occurs when the CPU needs a page that is not in RAM; that page must be brought in from disk (**page fault service time**), which is slow.
- **Page replacement algorithms** decide which page in RAM to evict when a new page must be brought in. One of them is **LRU**: evict the **least recently used** page, under the assumption that recently used pages are more likely to be used again soon.
- Other algorithms include **FIFO**, **Optimal**, and **MRU**. This project implements the **data structure** that supports an LRU-style policy (evict least recently used when full).

---

## Code Overview

### Class structure

The main implementation lives in `include/LRU_Cache.hpp`:

```cpp
class LRU_Cache {
public:
    LRU_Cache(int cap);
    int size() const;
    void feedin(int key, const std::string& data);
    std::string gettin(int key) const;
private:
    std::list<std::string> L;                           // front = MRU, back = LRU
    std::unordered_map<int, std::list<std::string>::iterator> M;
    int capacity;
};
```

### Constructor

- Checks `cap > 0`; if not, prints an error and leaves capacity as 0.
- Clears `L` and `M`.

### `feedin(key, data)`

- **Key not in cache:**  
  - If `L.size() == capacity`, find the key that points to the last element in `L`, erase it from `M`, then `L.pop_back()`.  
  - Then `L.push_front(data)` and `M[key] = L.begin()`.
- **Key already in cache:**  
  - `L.erase(M[key])`, then `L.push_front(data)` and `M[key] = L.begin()`.

### `gettin(key)`

- If key is not in `M`, return `"0"`.
- Otherwise return `*M[key]` (the string at that iterator).

---

## Notes

- **Eviction policy:** Eviction is based on **insertion/update order** (when each key was last **written**), not on **read** order. So `gettin(key)` does **not** move that key to “most recently used” in the list. If you want classic “LRU by access,” a `gettin` that finds the key could move its node to the front and update `M[key]`.
- The original single-file version with detailed comments is in `LRU Cache Project File.cpp`. The refactored implementation is in `include/LRU_Cache.hpp` and `src/main.cpp`.

---

## Is the code working?

**On GitHub:** The [badge](#lru-cache-implementation) at the top of this README shows the status of the **Build and Test** workflow. After you push and replace `YOUR_USERNAME` in the badge URL, it will show **passing** (green) when build and all 10 tests succeed. You can also open the **Actions** tab in your repo to see the latest run and logs.

**Locally:** Build and run the test suite. If you see `Passed: 10  Failed: 0` and exit code 0, the code is working.

```bash
# Linux/macOS
make all && ./test_lru_cache

# Or manually
g++ -std=c++17 -Wall -I. -o test_lru_cache tests/test_lru_cache.cpp && ./test_lru_cache
```

```powershell
# Windows (PowerShell)
.\build.ps1
```

Expected test output when everything works:

```
LRU Cache test suite
--------------------
  test_capacity_and_size ... OK
  test_single_insert_and_get ... OK
  ... (all 10 tests OK)
--------------------
Passed: 10  Failed: 0
```

---

## For GitHub

- **Badge:** Replace `YOUR_USERNAME` in the README badge URL with your GitHub username so the badge reflects your repo’s workflow status.
- Run the test suite before pushing: `.\test_lru_cache.exe` (Windows) or `./test_lru_cache` (Linux/macOS). Ensure all 10 tests pass.
- Use `build.ps1` (Windows) or `make all && make run-test` (Linux/macOS) to build and verify in one step.
- The `.gitignore` excludes build artifacts (`*.exe`, `*.o`, etc.); do not commit binaries.

---

*Project by Aditya Paneru — DSA, OOP, OS.*
