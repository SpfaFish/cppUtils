#pragma once

#include "list.hpp"
#include <stack>

//K is V's view
//(K)V must exist
template <typename K, typename V>
class LFU {
    struct Val {
        Val() {
            cnt = 0;
        }
        Val(const V& value) {
            this->value = value;
            cnt = 0;
        }
        V value;
        size_t cnt;
    };
  public:
    LFU() = delete;
    LFU(size_t capacity): capacity_(capacity) {
        freq_to_list_[0] = new List<Val>();
        mem.resize(capacity);
        for (int i = capacity - 1; i >= 0; i--) {
            s.push(i);
            mem[i] = "-";
        }
    }
    ~LFU() {
        for(auto& [k, v] : freq_to_list_) {
            delete v;
        }
    }
    void put(const K& key, const V& value) {
        auto it = tp_.find(key);
        if (it != tp_.end()) {
            auto& obj = it->second->obj_;
            it->second->extract();
            obj.value = value;
            ++obj.cnt;
            if (obj.cnt > max_freq_) {
                max_freq_ = obj.cnt;
                freq_to_list_[max_freq_] = new List<Val>();
            }
            freq_to_list_[obj.cnt]->insert(it->second);
        } else {
            if (tp_.size() == capacity_) {
                while (freq_to_list_[min_freq_]->empty()) {
                    ++min_freq_;
                }
                auto& list = freq_to_list_[min_freq_];
                auto& tmp = list->dummy_->prev_;
                mem[mem_map[(K)tmp->obj_.value]] = "-";
                s.push(mem_map[(K)tmp->obj_.value]);
                mem_map.erase((K)tmp->obj_.value);
                tp_.erase((K)tmp->obj_.value);
                list->erase(tmp);
            }
            auto* node = new Node<Val>(Val(value));
            min_freq_ = 0;
            tp_[key] = node;
            freq_to_list_[0]->insert(node);
            mem[s.top()] = std::to_string(key);
            mem_map[key] = s.top(); s.pop();
        }
    }
    bool get(const K& key, V& value) {
        auto it = tp_.find(key);
        if (it == tp_.end()) {
            return false;
        }
        auto& obj = it->second->obj_;
        value = obj.value;
        it->second->extract();
        ++obj.cnt;
        if (obj.cnt > max_freq_) {
            max_freq_ = obj.cnt;
            freq_to_list_[max_freq_] = new List<Val>();
        }
        freq_to_list_[obj.cnt]->insert(it->second);
        return true;
    }
    std::vector<std::string> getMemory() {
        return mem;
    }
  private:
    size_t capacity_;
    size_t min_freq_ = 0;
    size_t max_freq_ = 0;
    std::unordered_map<int, List<Val>*> freq_to_list_;
    std::unordered_map<K, Node<Val>*> tp_;
    std::stack<int> s;
    std::vector<std::string> mem;
    std::unordered_map<K, int> mem_map;
};