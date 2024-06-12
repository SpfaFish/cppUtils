#pragma once

#include "list.hpp"
#include <cassert>
#include <stack>

template <typename K, typename V>
class FIFO {
  public:
    FIFO() = delete;
    FIFO(size_t capacity): capacity_(capacity) {
        assert(capacity > 0);
        mem.resize(capacity);
        for (int i = capacity - 1; i >= 0; i--) {
            s.push(i);
            mem[i] = "-";
        }
    }
    void put(const K& key, const V& value) {
        auto it = tp.find(key);
        if (it != tp.end()) {
            return;
        } else {
            if (tp.size() == capacity_) {
                mem[mem_map[(K)list_->dummy_->prev_->obj_]] = "-";
                s.push(mem_map[(K)list_->dummy_->prev_->obj_]);
                mem_map.erase((K)list_->dummy_->prev_->obj_);
                tp.erase((K)list_->dummy_->prev_->obj_);
                list_->erase(list_->dummy_->prev_);
            }
            tp[key] = list_->insert(value);
            mem[s.top()] = std::to_string(key);
            mem_map[key] = s.top(); s.pop();
        }
    }
    bool get(const K& key, V& value) {
        auto it = tp.find(key);
        if (it != tp.end()) {
            value = (it->second->obj_);
            return true;
        } else {
            return false;
        }
    }
    std::vector<std::string> getMemory() {
        return mem;
    }
  private:
    int capacity_;
    std::unordered_map<K, Node<V>*> tp;
    std::shared_ptr<List<V>> list_ = std::make_shared<List<V>>();
    std::stack<int> s;
    std::vector<std::string> mem;
    std::unordered_map<K, int> mem_map;
};