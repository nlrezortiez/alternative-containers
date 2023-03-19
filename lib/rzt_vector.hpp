#pragma once

#include <cstddef>
#include <iterator>
#include <memory>
#include <stdexcept>

// TODO:     implement class-member funcs
//           must satisfy AllocatorAwareContainer Policy
//           fix problem given in test/libtst.cpp   

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

        template <typename U>
        struct common_iterator {
        public:
            using difference_type = typename allocator_traits::difference_type;
            using value_type = typename allocator_traits::value_type;
            using reference = U&;
            using const_reference = const U&;
            using pointer = typename allocator_traits::pointer;
            using iterator_category = std::random_access_iterator_tag;

            common_iterator() : ptr_() {
            }
            common_iterator(U* ptr) : ptr_(ptr) {
            }
            common_iterator(const common_iterator& other) : ptr_(other.ptr_) {
            }

            reference operator*() const {
                return *ptr_;
            }

            pointer operator->() {
                return ptr_;
            }

            reference operator[](size_type n) const {
                return *(ptr_ + n);
            }

            common_iterator& operator=(const common_iterator& other) {
                if (this != &other) {
                    ptr_ = other.ptr_;
                }
                return *this;
            }

            common_iterator& operator++() {
                ++ptr_;
                return *this;
            }

            common_iterator operator++(int) {
                return common_iterator(ptr_++);
            }

            common_iterator& operator--() {
                --ptr_;
                return *this;
            }

            common_iterator operator--(int) {
                return common_iterator(ptr_--);
            }

            common_iterator& operator+=(size_type n) {
                ptr_ += n;
                return *this;
            }

            common_iterator operator+(size_type n) const {
                return common_iterator(ptr_ + n);
            }

            common_iterator& operator-=(size_type n) {
                ptr_ -= n;
                return *this;
            }

            common_iterator operator-(size_type n) const {
                return common_iterator(ptr_ - n);
            }

            difference_type operator-(const common_iterator& other) const {
                return ptr_ - other.ptr_;
            }

            bool operator==(const common_iterator& rhs) const {
                return ptr_ == rhs.ptr_;
            }

            bool operator!=(const common_iterator& rhs) const {
                return !(ptr_ == rhs.ptr_);
            }

            bool operator<(const common_iterator& rhs) const {
                return (*this != rhs && *this - rhs < 0);
            }

            bool operator<=(const common_iterator& rhs) const {
                return (*this - rhs >= 0);
            }

            bool operator>(const common_iterator& rhs) const {
                return rhs < *this;
            }

            bool operator>=(const common_iterator& rhs) const {
                return rhs <= *this;
            }

            friend common_iterator operator+(size_type lhs, const common_iterator& rhs) {
                return common_iterator(rhs + lhs);
            }

        private:
            U* ptr_;
        };

        typedef common_iterator<T> iterator;
        typedef common_iterator<const T> const_iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

        vector() : data_start_(), data_end_(), storage_end_() {
        }
        explicit vector(size_type n, const_reference value = T(), const Allocator& alloc = Allocator()) : vector() {
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

        reference operator[](size_type index);
        const_reference operator[](size_type index) const;
        reference at(size_t index);
        const_reference at(size_t index) const;

        void clear();
        iterator insert(const_iterator pos, const_reference value);
        iterator insert(const_iterator pos, value_type&& value);
        iterator insert(const_iterator pos, size_type count, const_reference value);
        iterator insert(const_iterator pos, size_type count, value_type&& value);

        template <typename... Args>
        iterator emplace(const_iterator pos, Args&&... args);

        template <typename iterator_type>
        void erase(iterator_type pos);

        void push_back(const_reference value); // T -> CopyInsertable && MoveInsertable
        //void push_back(value_type&& value);

        template <typename... Args>
        reference emplace_back(Args&&... args);

        void pop_back();

    private:
        pointer data_start_;
        pointer data_end_;
        pointer storage_end_;

        allocator_type allocator;
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
    typename vector<T, Allocator>::size_type vector<T, Allocator>::capacity() const {
        return storage_end_ - data_start_;
    }

    template <typename T, typename Allocator>
    typename vector<T, Allocator>::const_reference vector<T, Allocator>::at(size_t index) const {
        if (index > size())
            throw std::out_of_range("out_of_range");
        return *(data_start_ + index);
    }

    template <typename T, typename Allocator>
    typename vector<T, Allocator>::reference vector<T, Allocator>::at(size_t index) {
        if (index > size())
            throw std::out_of_range("out_of_range");
        return *(data_start_ + index);
    }

    template <typename T, typename Allocator>
    typename vector<T, Allocator>::reference vector<T, Allocator>::operator[](size_type index) {
        return *(data_start_ + index);
    }

    template <typename T, typename Allocator>
    typename vector<T, Allocator>::const_reference vector<T, Allocator>::operator[](size_type index) const {
        return *(data_start_ + index);
    }

    template <typename T, typename Allocator>
    void vector<T, Allocator>::reserve(size_t new_capacity) {
        if (new_capacity < size())
            return;

        T* new_storage = allocator_traits::allocate(allocator, new_capacity); // possible exception
        size_type i = 0;
        try {
            for (; i < new_capacity; ++i) {
                allocator_traits::construct(allocator, new_storage + i, T());
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
            if (capacity() > 0) {
                reserve(2 * capacity());
            } else {
                reserve(1);
            }
        }
        allocator_traits::construct(allocator, data_end_++, value);
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
    typename vector<T, Allocator>::const_iterator vector<T, Allocator>::begin() const {
        return rzt::vector<T, Allocator>::const_iterator(data_start_);
    }

    template <typename T, typename Allocator>
    typename vector<T, Allocator>::const_iterator vector<T, Allocator>::end() const {
        return rzt::vector<T, Allocator>::const_iterator(data_end_);
    }

    template <typename T, typename Allocator>
    typename vector<T, Allocator>::const_iterator vector<T, Allocator>::cbegin() const {
        return rzt::vector<T, Allocator>::const_iterator(data_start_);
    }

    template <typename T, typename Allocator>
    typename vector<T, Allocator>::const_iterator vector<T, Allocator>::cend() const {
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
    typename vector<T, Allocator>::const_reverse_iterator vector<T, Allocator>::crbegin() const {
        return rzt::vector<T, Allocator>::const_reverse_iterator(data_end_);
    }

    template <typename T, typename Allocator>
    typename vector<T, Allocator>::const_reverse_iterator vector<T, Allocator>::crend() const {
        return rzt::vector<T, Allocator>::const_reverse_iterator(data_start_);
    }

} // namespace rzt
