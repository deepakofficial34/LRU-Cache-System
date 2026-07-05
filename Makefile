# LRU Cache project - build and test
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I.

.PHONY: all demo test clean

all: demo test

demo: lru_cache
test: test_lru_cache

lru_cache: src/main.cpp include/LRU_Cache.hpp
	$(CXX) $(CXXFLAGS) -o lru_cache src/main.cpp

test_lru_cache: tests/test_lru_cache.cpp include/LRU_Cache.hpp
	$(CXX) $(CXXFLAGS) -o test_lru_cache tests/test_lru_cache.cpp

clean:
	rm -f lru_cache test_lru_cache *.o *.exe

run: lru_cache
	./lru_cache

run-test: test_lru_cache
	./test_lru_cache
