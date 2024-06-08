#pragma once

#include "list.hpp"
#include <cassert>

template <typename K, typename V>
class FIFO {
  public:
    FIFO() = delete;
    FIFO(size_t capacity): capacity_(capacity) {
        assert(capacity > 0);
    }
    void put(const K& key, const V& value) {
        auto it = tp.find(key);
        if (it != tp.end()) {
            return;
        } else {
            if (tp.size() == capacity_) {
                tp.erase((K)list_->dummy_->prev_->obj_);
                list_->erase(list_->dummy_->prev_);
            }
            tp[key] = list_->insert(value);
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
  private:
    int capacity_;
    std::unordered_map<K, Node<V>*> tp;
    std::shared_ptr<List<V>> list_ = std::make_shared<List<V>>();
};