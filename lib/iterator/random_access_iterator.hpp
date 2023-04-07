#include <cstddef>
#include <iterator>

template <typename T>
struct common_iterator {
public:
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using iterator_category = std::random_access_iterator_tag;

    common_iterator() : ptr_() {
    }
    common_iterator(T* ptr) : ptr_(ptr) {
    }
    common_iterator(const common_iterator& other) : ptr_(other.ptr_) {
    }

    operator common_iterator<const T>() {
        return common_iterator<const T>(ptr_);
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

    pointer base() const {
        return ptr_;
    }

private:
    T* ptr_;
};
