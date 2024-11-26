#pragma once
#include <vector>
namespace qol {
template <class Ty>
class flat_stack {
public:
    using container = std::vector<Ty>;
    using iterator = typename container::iterator;
    using const_iterator = typename container::const_iterator;
    using reverse_iterator = typename container::reverse_iterator;
    using const_reverse_iterator = typename container::const_reverse_iterator;
    iterator begin() { return data_.begin(); }
    const_iterator begin() const { return data_.begin(); }
    iterator end() { return data_.end(); }
    const_iterator end() const { return data_.end(); }
    reverse_iterator rbegin() { return data_.rbegin(); }
    const_reverse_iterator rbegin() const { return data_.rbegin(); }
    reverse_iterator rend() { return data_.rend(); }
    const_reverse_iterator rend() const { return data_.rend(); }
    Ty& at(size_t idx) {return data_.at(idx);}
    const Ty& at(size_t idx) const {return data_.at(idx);}
    Ty& operator[](size_t idx) {return data_[idx];}
    Ty& emplace(Ty&& v) {return data_.emplace_back(std::forward<Ty&&>(v));}
    void push(const Ty& v) {data_.push_back(v);}
    void pop() {data_.pop_back();}
    Ty& top() {return data_.back();}
    size_t size() const {data_.size();}
    void reserve(size_t amount) {data_.reserve(amount);}
    bool empty() const {return data_.empty();}
    size_t capacity() const {return data_.capacity();}
private:
    std::vector<Ty> data_;
};
}
