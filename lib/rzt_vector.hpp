#pragma once

#include "iterator/random_access_iterator.hpp"

#include <memory>
#include <stdexcept>

// TODO:     rvalue-require methods, multiple values
// insert().
//           implement constructors;

namespace rzt {
template <typename T, typename Allocator = std::allocator<T>>
class vector {
public:
    using allocator_traits = std::allocator_traits<Allocator>;
    using allocator_type = typename allocator_traits::allocator_type;
    using value_type = typename allocator_traits::value_type;
    using reference = T&;
    using const_reference = const T&;
    using pointer = typename allocator_traits::pointer;
    using difference_type = typename allocator_traits::difference_type;
    using size_type = typename allocator_traits::size_type;

    using iterator = common_iterator<T>;
    using const_iterator = common_iterator<const T>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    vector() : data_start_(), data_end_(), storage_end_() {}
    explicit vector(size_type n, const_reference value = T(),
                    const Allocator& alloc = Allocator())
        : vector() {
        resize(n, value);
    }

    vector(const vector& other);
    vector(const vector& other, const Allocator& alloc);

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;
    reverse_iterator rbegin();
    const_reverse_iterator crbegin() const;
    reverse_iterator rend();
    const_reverse_iterator crend() const;

    [[nodiscard]] bool empty() const;
    size_type size() const;
    void reserve(size_t new_capacity);
    size_type capacity() const;
    void resize(size_t n, const T& value = T());
    void clear();

    reference operator[](size_type index);
    const_reference operator[](size_type index) const;
    reference at(size_t index);
    const_reference at(size_t index) const;

    size_type find(const T& value);

    iterator insert(const_iterator pos, const_reference value);
    // iterator insert(const_iterator pos, value_type&& value);
    // iterator insert(const_iterator pos, size_type count, const_reference
    // value); iterator insert(const_iterator pos, size_type count, value_type&&
    // value);

    template <typename... Args>
    iterator emplace(const_iterator pos, Args&&... args);

    template <typename iterator_type>
    void erase(iterator_type pos);

    void
    push_back(const_reference value); // T -> CopyInsertable && MoveInsertable
    // void push_back(value_type&& value);

    template <typename... Args>
    reference emplace_back(Args&&... args);

    void pop_back();

protected:
    pointer data_start_;
    pointer data_end_;
    pointer storage_end_;

    allocator_type allocator; // allocator awareness
};

template <typename T, typename Allocator>
typename vector<T, Allocator>::size_type vector<T, Allocator>::size() const {
    return data_end_ == nullptr ? 0 : data_end_ - data_start_;
}

template <typename T, typename Allocator>
bool vector<T, Allocator>::empty() const {
    return begin() == end();
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::size_type
vector<T, Allocator>::capacity() const {
    return storage_end_ - data_start_;
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::const_reference
vector<T, Allocator>::at(size_t index) const {
    if (index > size())
        throw std::out_of_range("out_of_range");
    return *(data_start_ + index);
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::reference
vector<T, Allocator>::at(size_t index) {
    if (index > size())
        throw std::out_of_range("out_of_range");
    return *(data_start_ + index);
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::reference
vector<T, Allocator>::operator[](size_type index) {
    return *(data_start_ + index);
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::const_reference
vector<T, Allocator>::operator[](size_type index) const {
    return *(data_start_ + index);
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::size_type
vector<T, Allocator>::find(const T& value) {
    for (int i = 0; i < this->size(); ++i) {
        if (value == this[i])
            return i;
    }
    return -1;
}

template <typename T, typename Allocator>
void vector<T, Allocator>::reserve(size_t new_capacity) {
    if (new_capacity < this->size())
        return;
    T* new_storage;
    try {
        new_storage = allocator_traits::allocate(
            allocator, new_capacity); // possible exception
    } catch (...) {
        throw;
    }

    size_type i = 0;
    try {
        for (; i < new_capacity; ++i) {
            if (i < size()) {
                allocator_traits::construct(allocator, new_storage + i,
                                            *(data_start_ + i));
            } else {
                allocator_traits::construct(allocator, new_storage + i, T());
            }
        }
    } catch (...) {
        for (size_t j = 0; j < i; ++j) {
            allocator_traits::destroy(allocator, new_storage + j);
        }
        allocator_traits::deallocate(allocator, new_storage, new_capacity);
        throw;
    }
    for (i = 0; i < size(); ++i) {
        allocator_traits::destroy(allocator, data_start_ + i);
    }
    allocator_traits::deallocate(allocator, data_start_, i); // i == size();

    data_start_ = new_storage;
    data_end_ = data_start_ + i;
    storage_end_ = data_start_ + new_capacity;
}

template <typename T, typename Allocator>
void vector<T, Allocator>::push_back(const_reference value) {
    if (data_end_ == storage_end_) {
        capacity() > 0 ? reserve(capacity() * 2) : reserve(1);
    }
    allocator_traits::construct(allocator, data_end_++, value);
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::iterator
vector<T, Allocator>::insert(const_iterator pos, const_reference value) {

    const size_type index = pos - begin();

    if (this->data_end_ == this->storage_end_) {
        capacity() > 0 ? reserve(2 * capacity()) : reserve(1);
    } // pos invalid after this => use index-logic only

    if (begin() + index == end()) {
        allocator_traits::construct(allocator, this->data_end_, value);
        ++this->data_end_;
    } else {
        for (size_type i = size() - 1; i >= index; --i) {
            allocator_traits::construct(allocator, this->data_start_ + i + 1,
                                        *(this->data_start_ + i));
        }
        allocator_traits::construct(allocator, this->data_start_ + index,
                                    value);
        ++this->data_end_;
    }

    return this->data_start_ + index;
}

template <typename T, typename Allocator>
void vector<T, Allocator>::pop_back() {
    allocator_traits::destroy(allocator, data_end_--);
}

template <typename T, typename Allocator>
void vector<T, Allocator>::resize(std::size_t n, const_reference value) {
    if (n > capacity()) {
        size_t i = size();
        reserve(capacity() + n);
        data_end_ = data_start_ + n;
        for (; i < n; ++i) {
            allocator_traits::construct(allocator, data_start_ + i, value);
        }
        storage_end_ = data_end_ = data_start_ + n;
    } else {
        if (n > size()) {
            for (int i = size(); i < n; ++i) {
                allocator_traits::construct(allocator, data_start_ + i, value);
            }
            data_end_ = data_start_ + n;
        } else {
            for (int i = n; i < size(); ++i) {
                allocator_traits::destroy(allocator, data_start_ + i);
            }
            data_end_ = data_start_ + n;
        }
    }
}

template <typename T, typename Allocator>
template <typename iterator_type>
void vector<T, Allocator>::erase(iterator_type pos) {
    if (pos.base() < data_start_ || pos.base() > data_end_) {
        return; //exception probably
    }
    size_type index = pos - this->begin();
    for (int i = index; i < size() - 1; ++i) {
        allocator_traits::construct(allocator, data_start_ + i, *(data_start_ + i + 1));
    }
    allocator_traits::destroy(allocator, data_end_--);
}

template <typename T, typename Allocator>
void vector<T, Allocator>::clear() {
    for (int i = 0; i < size(); ++i) {
        allocator_traits::destroy(allocator, data_start_ + i);
    }
    data_end_ = data_start_;
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::begin() {
    return rzt::vector<T, Allocator>::iterator(data_start_);
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::end() {
    return rzt::vector<T, Allocator>::iterator(data_end_);
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::const_iterator
vector<T, Allocator>::begin() const {
    return rzt::vector<T, Allocator>::const_iterator(data_start_);
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::const_iterator
vector<T, Allocator>::end() const {
    return rzt::vector<T, Allocator>::const_iterator(data_end_);
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::const_iterator
vector<T, Allocator>::cbegin() const {
    return rzt::vector<T, Allocator>::const_iterator(data_start_);
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::const_iterator
vector<T, Allocator>::cend() const {
    return rzt::vector<T, Allocator>::const_iterator(data_end_);
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::reverse_iterator vector<T, Allocator>::rbegin() {
    return rzt::vector<T, Allocator>::reverse_iterator(data_end_);
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::reverse_iterator vector<T, Allocator>::rend() {
    return rzt::vector<T, Allocator>::reverse_iterator(data_start_);
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::const_reverse_iterator
vector<T, Allocator>::crbegin() const {
    return rzt::vector<T, Allocator>::const_reverse_iterator(data_end_);
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::const_reverse_iterator
vector<T, Allocator>::crend() const {
    return rzt::vector<T, Allocator>::const_reverse_iterator(data_start_);
}

} // namespace rzt
