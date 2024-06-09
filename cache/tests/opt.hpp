#include <algorithm>
#include <atomic>
#include <cstddef>
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
            return value == rhs.value ? id < rhs.id : value < rhs.value; 
        }
    };
    OPT() = delete;
    OPT(int capacity): capacity_(capacity) {
        assert(capacity > 0);
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
    double calculate(const std::vector<V>& reqs) {
        __preWork(reqs);
        int64_t cnt = 0;
        std::set<node> s;
        std::vector<size_t> ptr(obj_list_.size(), -1);
        for (size_t i = 0; i < reqs.size(); i++) {
            auto it = st.find(static_cast<K>(reqs[i]));
            if (it != st.end()) {
                ++cnt;
                auto& p = ++ptr[values[i]];
                auto& list = obj_list_[values[i]];
                if (p > 0) {
                    s.erase(node(static_cast<K>(reqs[i]), p == list.size() ? inf : list[p] - list[p - 1], p - 1));
                }
                if (p < list.size()) {
                    s.insert(node(static_cast<K>(reqs[i]), p + 1 == list.size() ? inf : list[p + 1] - list[p], p));
                }
            } else {
                if (st.size() == capacity_) {
                    st.erase(s.begin()->key);
                    s.erase(s.begin());
                }
                st.insert(static_cast<K>(reqs[i]));
                auto& p = ++ptr[values[i]];
                auto& list = obj_list_[values[i]];
                if (p < list.size()) {
                    s.insert(node(static_cast<K>(reqs[i]), p + 1 == list.size() ? inf : list[p + 1] - list[p], p));
                }
            }
        }
        return cnt / static_cast<double>(reqs.size());
    }
  private:
    int capacity_;
    std::vector<K> p;
    std::vector<size_t> values;
    std::vector<size_t> id;
    std::vector<std::vector<size_t> > obj_list_;
    std::set<K> st;
};

#undef inf 