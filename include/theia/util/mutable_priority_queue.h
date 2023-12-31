// Please contact the author of this library if you have any questions.
// Author: Chris Sweeney (cmsweeney@cs.ucsb.edu)

#ifndef THEIA_UTIL_MUTABLE_PRIORITY_QUEUE_H_
#define THEIA_UTIL_MUTABLE_PRIORITY_QUEUE_H_

#include <algorithm>
#include <functional>
#include <vector>
#include <unordered_map>
#include <utility>

#include "theia/util/map_util.h"
#include "theia/util/util.h"

namespace theia {

// A mutable priority queue (MPQ) that may be easily customized with an STL-like
// interface. This MPQ is useful for scenarios where values are often changing
// and you would like to always have a sorted list of the values. By default
// this is a min-heap that will put the smaller values at the top. However, this
// may be easily customized by providing a method ValueComp to perform the
// element-wise comparison.
template <typename Key,
          typename Value,
          typename ValueComp = std::greater<Value> >
class mutable_priority_queue {
 private:
  typedef std::pair<Key, Value> KeyValuePair;

  static bool KeyValuePtrComp(KeyValuePair* kv1, KeyValuePair* kv2) {
    ValueComp cmp;
    return cmp(kv1->second, kv2->second);
  }

 public:
  // Default constructor.
  mutable_priority_queue() {}

  // Copy constructor.
  mutable_priority_queue(const mutable_priority_queue<Key, Value, ValueComp>& x)
      : heap_(x.heap_), value_index_(x.value_index) {}

  // Destructor, clears both internal maps.
  ~mutable_priority_queue() { STLDeleteElements(&heap_); }

  // Assignment operator.
  inline void operator=(mutable_priority_queue<Key, Value, ValueComp> x) {
    heap_ = x.heap_;
    value_index_ = x.value_index_;
  }

  inline void reserve(const int size) {
    heap_.reserve(size);
    value_index_.reserve(size);
  }

  // Empties the queue.
  inline void clear() {
    STLDeleteElements(&heap_);
    value_index_.clear();
  }

  // Removes from both maps the value val.
  inline void erase(const Key& key) {
    if (!ContainsKey(value_index_, key)) {
      return;
    }

    KeyValuePair* kv = FindOrDie(value_index_, key);
    value_index_.erase(key);
    auto it = std::find(heap_.begin(), heap_.end(), kv);
    heap_.erase(it);
    std::make_heap(heap_.begin(), heap_.end(), KeyValuePtrComp);
    delete kv;
  }

  // Returns true if the queue is empty.
  inline bool empty() { return heap_.size() == 0; }

  inline std::pair<Key, Value>& top() const { return *heap_.front(); }

  // Removes the front entry in the queue.
  inline void pop() {
    KeyValuePair* kv = heap_.front();
    value_index_.erase(kv->first);
    delete kv;

    std::pop_heap(heap_.begin(), heap_.end(), KeyValuePtrComp);
    heap_.pop_back();
  }

  // Push an entry onto the priority queue.
  inline void insert(const Key& key, const Value& value) {
    KeyValuePair* kv = new KeyValuePair(key, value);
    value_index_[key] = kv;
    heap_.push_back(kv);
    std::push_heap(heap_.begin(), heap_.end(), KeyValuePtrComp);
  }

  // Update an entry within the priority queue and move it to its proper
  // position.
  inline void update(const Key& key, const Value& value) {
    KeyValuePair* kv = FindOrDie(value_index_, key);
    kv->second = value;
    std::make_heap(heap_.begin(), heap_.end(), KeyValuePtrComp);
  }

  // Returns the number of elements in the queue.
  inline size_t size() {
    DCHECK_EQ(heap_.size(), value_index_.size());
    return heap_.size();
  }

  inline bool contains(const Key& key) {
    return ContainsKey(value_index_, key);
  }

  // Returns the value for the key.
  inline Value& find(const Key& key) {
    return FindOrDie(value_index_, key)->second;
  }

 private:
  std::vector<KeyValuePair*> heap_;
  std::unordered_map<Key, KeyValuePair*> value_index_;
};

}  // namespace theia

#endif  // THEIA_UTIL_MUTABLE_PRIORITY_QUEUE_H_
