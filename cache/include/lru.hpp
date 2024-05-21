#pragma once

#include <cassert>
#include <cstddef>
#include <iostream> 
#include <memory>

#include "list.hpp"

//K is V's view
//(K)V must exist
template <typename K, typename V>
class LRU {
  public:
    LRU(int capacity): capacity_(capacity) {
        assert(capacity > 0);
    }
    void put(const K& key, const V& value) {
        auto it = tp.find(key);
        if (it != tp.end()) {
            it->second->obj_ = value;
            list_->extract(it->second);
            list_->insert(it->second);
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
    int capacity_ = 10;
    std::unordered_map<K, Node<V>*> tp;
    std::shared_ptr<List<V>> list_ = std::make_shared<List<V>>();
    V NullValue;
};