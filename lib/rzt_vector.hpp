#pragma once

namespace rzt {
    template <typename T, typename Alloc = std::allocator<T>>
    class vector {
    public:
        using this_type = vector<T, Alloc>;
        using value_type = typename Alloc::value_type;
        using pointer = Alloc::value_type*;
        using const_pointer = const Alloc::value_type*;
        using reference = Alloc::value_type&;
        using const_reference = const Alloc::value_type&;
        using rvalue = Alloc::value_type&&;

        using difference_type = typename Alloc::difference_type;
        using size_t = typename Alloc::size_type;

        template <bool ISConst>
        struct common_iterator {
        public:
            common_iterator(pointer ptr) : ptr_(ptr) {
            }

            std::conditional_t<ISConst, const_reference, reference> operator*() {
                return *ptr_;
            }

            std::conditional<ISConst, const_pointer, pointer> operator->() {
                return ptr_;
            }

            common_iterator<ISConst>& operator++() {
                ++ptr_;
                return *this;
            }

            common_iterator<ISConst> operator++(int) {
                return common_iterator<ISConst>(ptr_++);
            }

        private:
            std::conditional_t<ISConst, const_pointer, pointer> ptr_;
        };

        using iterator = common_iterator<false>;
        using const_iterator = common_iterator<true>;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

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
        size_t size() const;
        void reserve(size_t new_capacity);
        size_t capacity() const;

        void clear();
        void insert();
        void emplace();
        void erase();
        void push_back(const_reference value); // T -> CopyInsertable && MoveInsertable
        void push_back(rvalue value);

        template <typename... Args>
        reference emplace_back(Args&&... args);

        void pop_back();
        void resize(size_t count);
        void resize(size_t count, const_reference value);

        reference at(size_t pos);
        const_reference at(size_t) const;

    private:
        pointer data_start_;
        pointer data_end_;
        pointer storage_end_;
    };
    template <typename T, typename Alloc>
    size_t vector<T, Alloc>::size() const {
        return std::distance(begin(), end());
    }

    template <typename T, typename Alloc>
    bool vector<T, Alloc>::empty() const {
        return size() == 0;
    }

    template <typename T, typename Alloc>
    vector<T, Alloc>::iterator vector<T, Alloc>::begin() {
        return rzt::vector<T, Alloc>::iterator(data_start_);
    }

    template <typename T, typename Alloc>
    vector<T, Alloc>::iterator vector<T, Alloc>::end() {
        return rzt::vector<T, Alloc>::iterator(data_end_);
    }

    template <typename T, typename Alloc>
    vector<T, Alloc>::const_iterator vector<T, Alloc>::begin() const {
        return rzt::vector<T, Alloc>::const_iterator(data_start_);
    }

    template <typename T, typename Alloc>
    vector<T, Alloc>::const_iterator vector<T, Alloc>::end() const {
        return rzt::vector<T, Alloc>::const_iterator(data_end_);
    }

    template <typename T, typename Alloc>
    vector<T, Alloc>::const_iterator vector<T, Alloc>::cbegin() const {
        return rzt::vector<T, Alloc>::const_iterator(data_start_);
    }

    template <typename T, typename Alloc>
    vector<T, Alloc>::const_iterator vector<T, Alloc>::cend() const {
        return rzt::vector<T, Alloc>::const_iterator(data_end_);
    }

    template <typename T, typename Alloc>
    vector<T, Alloc>::reverse_iterator vector<T, Alloc>::rbegin() {
        return rzt::vector<T, Alloc>::reverse_iterator(data_end_);
    }

    template <typename T, typename Alloc>
    vector<T, Alloc>::reverse_iterator vector<T, Alloc>::rend() {
        return rzt::vector<T, Alloc>::reverse_iterator(data_start_);
    }

    template <typename T, typename Alloc>
    vector<T, Alloc>::const_reverse_iterator vector<T, Alloc>::crbegin() const {
        return rzt::vector<T, Alloc>::const_reverse_iterator(data_end_);
    }

    template <typename T, typename Alloc>
    vector<T, Alloc>::const_reverse_iterator vector<T, Alloc>::crend() const {
        return rzt::vector<T, Alloc>::const_reverse_iterator(data_start_);
    }

} // namespace rzt