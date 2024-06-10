#include <gflags/gflags.h>

#include "cache/include/fifo.hpp"
#include "cache/include/lru.hpp"
#include "cache/include/lfu.hpp"
#include "cache/include/opt.hpp"
DEFINE_int32(capacity, 1000, "cache capacity");
DEFINE_bool(cache, true, "cache enable");
DEFINE_double(interrupt_time, 100, "page fault interrupt time");
DEFINE_double(memory_access_time, 1, "memory_access_time");
DEFINE_int32(req_list_size, 10000, "request list size");
int main () {
    
}