#include <gtest/gtest.h>
#include "list.hpp"
#include "lru.hpp"
#include "lfu.hpp"
#include "fifo.hpp"

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
    std::cout << "here" << std::endl;
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
int main () {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}