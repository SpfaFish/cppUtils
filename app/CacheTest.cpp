#include <cctype>
#include <random>
#include <string>
#include <vector>
#include <tuple>

#include <gflags/gflags.h>

#include "cache/include/fifo.hpp"
#include "cache/include/lru.hpp"
#include "cache/include/lfu.hpp"
#include "cache/include/opt.hpp"
DEFINE_int32(capacity, 10, "cache capacity");
DEFINE_bool(cache, true, "cache enable");
DEFINE_double(interrupt_time, 100, "page fault interrupt time, ms");
DEFINE_double(memory_access_time, 1, "memory_access_time, ms");
DEFINE_int32(req_list_size, 100, "request list size");
DEFINE_string(req_list, "", "request list");
DEFINE_int32(max_range, 20, "max range");
DEFINE_bool(output_each_cost, false, "output each cost");

std::vector<int> randSeq(int size, int mx) {
    thread_local std::mt19937 gen(std::random_device{}());
    std::vector<int> res(size);
    for (int i = 0; i < size; i++) {
        res[i] = gen() % FLAGS_max_range;
    }
    return res;
}
// 命中率，平均访问时间，每次访问时间
template <typename Cache>
std::tuple<double, double, std::vector<double>, std::vector<std::vector<std::string> > > commonTest(const std::vector<int>& seq, int capacity) {
    Cache cache(capacity);
    double hit = 0;
    double cost = 0;
    std::vector<double> res;
    res.reserve(seq.size());
    std::vector<std::vector<std::string> > data;
    data.reserve(seq.size());
    int not_hit = 0;
    for (auto& i : seq) {
        int v;
        // 双倍贡献
        bool flag = true;
        if (cache.get(i, v)) {
            ++hit;
            cost += FLAGS_memory_access_time;
            res.emplace_back(FLAGS_memory_access_time);
            cache.put(i, i);
        } else {
            ++not_hit;
            flag = false;
            cost += FLAGS_memory_access_time + FLAGS_interrupt_time;
            res.emplace_back(FLAGS_memory_access_time + FLAGS_interrupt_time);
            cache.put(i, i);
            cache.put(i, i);
        }
        auto tmp = cache.getMemory();
        tmp.insert(tmp.begin(), std::to_string(i));
        tmp.push_back(flag ? "Yes" : "No");
        tmp.push_back(std::to_string(not_hit));
        data.emplace_back(tmp);
    }
    return {static_cast<double>(hit) / seq.size(), cost, res, data};
}
std::tuple<double, double, std::vector<double>, std::vector<std::vector<std::string> > >  optTest(const std::vector<int>& seq, int capacity) {
    OPT<int, int> opt(capacity);
    return opt.calculate(seq, FLAGS_memory_access_time, FLAGS_interrupt_time);
}
std::vector<int> splitReqList(const std::string& req_list) {
    std::vector<int> res;
    for (int l = 0, r; l < req_list.size(); l = r) {
        while(l < req_list.size() && !std::isdigit(req_list[l])) ++l;
        r = l;
        int v = 0;
        while (r < req_list.size() && std::isdigit(req_list[r])) {
            v = (v << 3) + (v << 1) + (req_list[r] ^ 48);
            ++r;
        }
        res.emplace_back(v);
    }
    return res;
}
int main (int argc, char* argv[]) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    std::vector<int> seq;
    if (FLAGS_req_list.empty()) {
        seq = randSeq(FLAGS_req_list_size, FLAGS_max_range);
    } else {
        seq = splitReqList(FLAGS_req_list);
    }
    if (FLAGS_cache) {
        {
            auto [rate, cost, res, data] = commonTest<FIFO<int, int>>(seq, FLAGS_capacity);
            // std::cout << "[FIFO] hit rate: " << rate << " cost time: " << cost << "ms" << std::endl;
            for (int i = 0; i < data.size(); i++) {
                for(int j = 0;  j < data[i].size(); j++) {
                    std::cout << data[i][j] << " ";
                } std::cout << std::endl;
            }
            std::cout << "[FIFO]hitRate: " << rate << std::endl << "[FIFO]costTime: " << cost << std::endl;
            if (FLAGS_output_each_cost) {
                for (double x : res) {
                    std::cout << x << " ";
                } std::cout << std::endl;
            }
        }
        {
            auto [rate, cost, res, data]  = commonTest<LRU<int, int>>(seq, FLAGS_capacity);
            // std::cout << "[LRU] hit rate: " << rate << " cost time: " << cost << "ms" << std::endl;
            for (int i = 0; i < data.size(); i++) {
                for(int j = 0;  j < data[i].size(); j++) {
                    std::cout << data[i][j] << " ";
                } std::cout << std::endl;
            }
            std::cout << "[LRU]hitRate: " << rate << std::endl << "[LRU]costTime: " << cost << std::endl;
            if (FLAGS_output_each_cost) {
                for (double x : res) {
                    std::cout << x << " ";
                } std::cout << std::endl;
            }
        }
        {
            auto [rate, cost, res, data] = commonTest<LFU<int, int>>(seq, FLAGS_capacity);
            for (int i = 0; i < data.size(); i++) {
                for(int j = 0;  j < data[i].size(); j++) {
                    std::cout << data[i][j] << " ";
                } std::cout << std::endl;
            }
            // std::cout << "[LFU] hit rate: " << rate << " cost time: " << cost << "ms" << std::endl;
            std::cout << "[LFU]hitRate: " << rate << std::endl << "[LFU]costTime: " << cost << std::endl;
            if (FLAGS_output_each_cost) {
                for (double x : res) {
                    std::cout << x << " ";
                } std::cout << std::endl;
            }
        }
        {
            auto [rate, cost, res, data] = optTest(seq, FLAGS_capacity);
            for (int i = 0; i < data.size(); i++) {
                for(int j = 0;  j < data[i].size(); j++) {
                    std::cout << data[i][j] << " ";
                } std::cout << std::endl;
            }
            std::cout << "[OPT]hitRate: " << rate << std::endl << "[OPT]costTime: " << cost << std::endl;
            if (FLAGS_output_each_cost) {
                for (double x : res) {
                    std::cout << x << " ";
                } std::cout << std::endl;
            }
        }
    } else {
        std::cout << "no cache time cost: " << FLAGS_interrupt_time * seq.size() << "ms" << std::endl;
    }
}