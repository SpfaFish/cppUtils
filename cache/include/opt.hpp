#include <algorithm>
#include <atomic>
#include <cstddef>
#include <stack>
#include <string>
#include <vector>
#include <cassert>
#include <set>

#include "list.hpp"

#define inf 0x3f3f3f3f

template <typename K, typename V>
class OPT {
  public:
    struct node {
        K key;
        size_t value;
        uint32_t id;
        template <typename T>
        node(T&& key, size_t value, uint32_t cnt) {
            this->key = std::forward<T>(key);
            this->value = value;
            id = cnt;
        }
        bool operator < (const node& rhs) const {
            return value == rhs.value ? id < rhs.id : value > rhs.value; 
        }
    };
    OPT() = delete;
    OPT(int capacity): capacity_(capacity) {
        assert(capacity > 0);
        mem.resize(capacity);
        for (int i = capacity - 1; i >= 0; i--) {
            stk.push(i);
            mem[i] = "-";
        }
    }
    size_t __get(K key) {
        return std::lower_bound(p.begin(), p.end(), key) - p.begin();
    }
    void __preWork(const std::vector<V>& reqs) {
        // std::vector<K> p(reqs.size());
        p.resize(reqs.size());
        for (int i = 0; i < p.size(); i++) {
            p[i] = static_cast<K>(reqs[i]);
        }
        std::sort(p.begin(), p.end());
        size_t size = std::unique(p.begin(), p.end()) - p.begin();
        p.resize(size);

        obj_list_.resize(p.size());
        values.resize(reqs.size());
        id.resize(reqs.size());
        for (size_t i = 0; i < reqs.size(); i++) {
            values[i] = __get(reqs[i]);
            id[i] = obj_list_[values[i]].size();
            obj_list_[values[i]].emplace_back(i);            
        }
    }
    // 算出命中率
    std::tuple<double, double, std::vector<double>, std::vector<std::vector<std::string> > > calculate(const std::vector<V>& reqs, double memory_access_time = 1, double interrupt_time = 100) {
        __preWork(reqs);
        int64_t cnt = 0;
        std::set<node> s;
        std::vector<size_t> ptr(obj_list_.size(), -1);
        double cost = 0;
        std::vector<double> res;
        res.reserve(reqs.size());
        std::vector<std::vector<std::string> > data;
        data.reserve(reqs.size());
        int not_hit = 0;
        for (size_t i = 0; i < reqs.size(); i++) {
            bool flag = true;
            auto it = st.find(static_cast<K>(reqs[i]));
            if (it != st.end()) {
                cost += memory_access_time;
                res.emplace_back(memory_access_time);
                ++cnt;
                auto& p = ++ptr[values[i]];
                auto& list = obj_list_[values[i]];
                if (p > 0) {
                    s.erase(node(static_cast<K>(reqs[i]), p == list.size() ? -inf : list[p] - list[p - 1], list[p - 1]));
                }
                if (p < list.size()) {
                    s.insert(node(static_cast<K>(reqs[i]), p + 1 == list.size() ? -inf : list[p + 1] - list[p], i));
                }
            } else {
                flag = false;
                ++not_hit;
                cost += memory_access_time + interrupt_time;
                res.emplace_back(memory_access_time + interrupt_time);
                if (st.size() == capacity_) {
                    // std::cout << i << " " << " out " << s.begin()->key << std::endl;
                    st.erase(s.begin()->key);
                    mem[mem_map[s.begin()->key]] = "-";
                    stk.push(mem_map[s.begin()->key]);
                    mem_map.erase(s.begin()->key);
                    s.erase(s.begin());
                }
                st.insert(static_cast<K>(reqs[i]));
                auto& p = ++ptr[values[i]];
                auto& list = obj_list_[values[i]];
                if (p < list.size()) {
                    s.insert(node(static_cast<K>(reqs[i]), p + 1 == list.size() ? inf : list[p + 1] - list[p], p));
                    mem[stk.top()] = std::to_string(static_cast<K>(reqs[i]));
                    mem_map[static_cast<K>(reqs[i])] = stk.top(); stk.pop();
                }
            }
            auto tmp = mem;
            tmp.insert(tmp.begin(), std::to_string((K)reqs[i]));
            tmp.push_back(flag ? "Yes" : "No");
            tmp.push_back(std::to_string(not_hit));
            data.emplace_back(std::move(tmp));
        }
        return {cnt / static_cast<double>(reqs.size()), cost, res, data};
    }
  private:
    int capacity_;
    std::vector<K> p;
    std::vector<size_t> values;
    std::vector<size_t> id;
    std::vector<std::vector<size_t> > obj_list_;
    std::set<K> st;
    std::stack<int> stk;
    std::vector<std::string> mem;
    std::unordered_map<K, int> mem_map;
};

#undef inf 