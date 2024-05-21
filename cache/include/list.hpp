#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>

template <typename T>
struct Node {
    Node(){}
    Node(const T& obj): obj_(obj) {}
    Node(T&& obj): obj_(std::forward<T>(obj)) {}
    T obj_;
    Node* prev_;
    Node* next_;
    void extract() {
        prev_->next_ = next_;
        next_->prev_ = prev_;
        prev_ = nullptr;
        next_ = nullptr;
    }
};

template <typename T>
class List {
  public:
    List() {
        dummy_ = new Node<T>();
        dummy_->prev_ = dummy_;
        dummy_->next_ = dummy_;
    }
    ~List() {
        std::vector<Node<T>*> nodes;
        nodes.reserve(size_ + 1);
        for (auto it = dummy_->next_; it != dummy_; it = it->next_) {
            nodes.push_back(it);
        }
        nodes.push_back(dummy_);
        for (auto& node: nodes) {
            delete node;
        }
    }
    void insert(Node<T>* new_node) {
        ++size_;
        new_node->prev_ = dummy_;
        new_node->next_ = dummy_->next_;
        dummy_->next_->prev_ = new_node;
        dummy_->next_ = new_node;
    }
    // if node is nullptr, insert to head, else insert obj to node's back
    Node<T>* insert(const T& obj, Node<T>* node = nullptr) {
        ++size_;
        if (node == nullptr) {
            Node<T>* new_node = new Node<T>(obj);
            new_node->prev_ = dummy_;
            new_node->next_ = dummy_->next_;
            dummy_->next_->prev_ = new_node;
            dummy_->next_ = new_node;
            return new_node;
        } else {
            Node<T>* new_node = new Node<T>(obj);
            new_node->prev_ = node;
            new_node->next_ = node->next_;
            node->next_->prev_ = new_node;
            node->next_ = new_node;
            return new_node;
        }
    }
    void extract(Node<T>* node) {
        --size_;
        node->extract();
    }
    void erase(Node<T>* node) {
        extract(node);
        delete node;
    }
    List(const List& rhs) {
        dummy_ = new Node<T>();
        dummy_->prev_ = dummy_;
        dummy_->next_ = dummy_;
        for (auto it = rhs.dummy_->next_; it != rhs.dummy_; it = it->next_) {
            insert(it->obj_);
        }
    }
    std::string toDebugString() {
        std::string res;
        for (auto it = dummy_->next_; it != dummy_; it = it->next_) {
            res += std::to_string(it->obj_) + " ";
        }
        return res;
    }
    size_t size() {
        return size_;
    }
    bool empty() {
        return dummy_->prev_ == dummy_;
    }
  public:
    Node<T>* dummy_ = nullptr;
  private:
    size_t size_ = 0;
};