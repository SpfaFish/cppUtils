#include <gtest/gtest.h>
#include <random>
#include "list.hpp"
#include "lru.hpp"
#include "lfu.hpp"
#include "fifo.hpp"
#include "opt.hpp"

TEST(ListTest, TestInsert) {
    List<int> list;
    list.insert(1);
    list.insert(2);
    list.insert(3);
    list.insert(4);
    list.insert(5);
    list.insert(6);
    list.insert(7);
    list.insert(8);
    list.insert(9);
    list.insert(10);
    assert(list.toDebugString() == "10 9 8 7 6 5 4 3 2 1 ");
}
TEST(LRUTest, Test) {
    LRU<int, int> lru(10);
    for(int i = 1; i <= 15; i++) {
        lru.put(i, i);
    }
    int v;
    if (lru.get(5, v) ) {
        std::cout << v << std::endl;
    }
    
    assert(!lru.get(5, v));

    assert(lru.get(10, v));
    lru.put(6, 6);
    lru.put(20, 20);
    assert(lru.get(6, v));
    assert(!lru.get(7, v));
}
TEST(LFUTest, Test) {
    LFU<int, int> lfu(10);
    for(int i = 1; i <= 15; i++) {
        lfu.put(i, i);
    }
    int v;
    assert(!lfu.get(5, v));
    lfu.get(6, v);
    for(int i = 20; i < 30; i++) {
        lfu.put(i, i);
    }
    assert(lfu.get(6, v));
}
TEST(FIFOTest, Test) {
    FIFO<int, int> fifo(10);
    for(int i = 1; i <= 15; i++) {
        fifo.put(i, i);
    }
    fifo.put(7, 7);
    int v;
    assert(!fifo.get(5, v));
    assert(fifo.get(6, v));
}
TEST(OPTTest, Test) {
    OPT<int, int> opt(5);
    std::cout << std::get<0>(opt.calculate(std::vector<int>{1, 2, 3, 4, 5, 6, 1, 2, 3, 4, 5})) << std::endl;
}
std::vector<int> randSeq(int size, int mx) {
    thread_local std::mt19937 gen(std::random_device{}());
    std::vector<int> res(size);
    for (int i = 0; i < size; i++) {
        res[i] = gen() % mx;
    }
    return res;
}
template <typename Cache>
double commonTest(const std::vector<int>& seq, int capacity) {
    Cache cache(capacity);
    double hit = 0;
    for (auto& i : seq) {
        int v;
        // 双倍贡献
        if (cache.get(i, v)) {
            ++hit;
            cache.put(i, i);
        } else {
            cache.put(i, i);
            cache.put(i, i);
        }
    }
    return static_cast<double>(hit) / seq.size();
}
double optTest(const std::vector<int>& seq, int capacity) {
    OPT<int, int> opt(capacity);
    return std::get<0>(opt.calculate(seq));
}
TEST(FinalTest, Test) {
    std::vector<int> seq = randSeq(10000, 1000);
    // std::cout << "seq: ";
    // for (int x : seq) {
    //     std::cout << x << " ";
    // } std::cout << std::endl;
    const int capacity = 500;
    std::cout << "FIFO: " << commonTest<FIFO<int, int>>(seq, capacity) << std::endl;
    std::cout << "LRU: " << commonTest<LRU<int, int>>(seq, capacity) << std::endl;
    std::cout << "LFU: " << commonTest<LFU<int, int>>(seq, capacity) << std::endl;
    std::cout << "OPT: " << optTest(seq, capacity) << std::endl;
}
int main () {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}