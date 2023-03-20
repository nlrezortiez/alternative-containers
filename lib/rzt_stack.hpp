#pragma once

#include <iterator>

namespace rzt {
    template <typename T>
    class ListNode {
      public:
        T data_;
        ListNode* next_;

        ListNode() : next_(nullptr) {
        }

        explicit ListNode(T data = T()) : data_(data), next_(nullptr) {
        }
    };

    template <typename T>
    struct list_iterator {
        typedef list_iterator this_type;
        typedef ListNode<T> list_node;
        typedef std::ptrdiff_t difference_type;
        typedef std::forward_iterator_tag iterator_category;

        typedef T value_type;
        typedef T* pointer;
        typedef T& reference;

        list_iterator() : ptr_() {
        }

        list_iterator(list_node* node) : ptr_(node) {
        }

        reference operator*() const {
            return static_cast<list_node*>(this->ptr_)->data_;
        }

        pointer operator->() const {
            return &static_cast<list_node*>(this->ptr_)->data_;
        }

        this_type& operator++() {
            ptr_ = ptr_->next_;
            return *this;
        }

        this_type operator++(int) {
            this_type tmp(*this); // const ref copy = default;
            ptr_ = ptr_->next_;
            return tmp;
        }

        friend bool operator==(const this_type& lhs, const this_type& rhs) {
            return lhs.ptr_ == rhs.ptr_;
        }

      private:
        list_node* ptr_;
    };

    template <typename T>
    struct const_list_iterator {
        typedef const_list_iterator this_type;
        typedef ListNode<T> list_node;
        typedef ptrdiff_t difference_type;
        typedef std::forward_iterator_tag iterator_category;

        typedef const T value_type;
        typedef const T* pointer;
        typedef const T& reference;

        const_list_iterator() : ptr_() {
        }

        const_list_iterator(const list_node* node) : ptr_(node) {
        }

        reference operator*() const {
            return static_cast<const list_node*>(this->ptr_)->data_;
        }

        pointer operator->() const {
            return &static_cast<const list_node*>(this->ptr_)->data_;
        }

        this_type& operator++() {
            ptr_ = ptr_->next_;
            return *this;
        }

        this_type operator++(int) {
            this_type tmp(*this); // const ref copy = default;
            ptr_ = ptr_->next_;
            return tmp;
        }

        friend bool operator==(const this_type& lhs, const this_type& rhs) {
            return lhs.ptr_ == rhs.ptr_;
        }

      private:
        const list_node* ptr_;
    };

} // namespace rzt

namespace rzt {
    template <typename T>
    class stack {
      public:
        typedef list_iterator<T> iterator;
        typedef const_list_iterator<T> const_iterator;

        stack() : top_(nullptr) {
        }

        ~stack();

        stack(const stack<T>& other) : stack() {
            stack<T> tmp;
            for (const T& item : other) {
                tmp.push(item);
            }
            for (const T& item : tmp) {
                this->push(item);
            }
        }

        [[nodiscard]] bool empty() const;

        void push(const T& n);

        void push(T&& n);

        void pop();

        uint64_t size() const;

        T& top() const;

        stack<T> reversed() const;

        iterator begin() {
            return iterator(top_);
        }

        iterator end() {
            return nullptr;
        }

        const_iterator begin() const {
            return const_iterator(top_);
        }

        const_iterator end() const {
            return const_iterator(nullptr);
        }

        const_iterator cbegin() const {
            return const_iterator(nullptr);
        }

        const_iterator cend() const {
            return const_iterator(nullptr);
        }

      private:
        ListNode<T>* top_;
    };

    template <typename T>
    stack<T>::~stack() {
        while (!this->empty()) {
            this->pop();
        }
    }

    template <typename T>
    void stack<T>::push(const T& n) {
        auto new_node = new ListNode<T>(n);
        new_node->next_ = top_;
        top_ = new_node;
    }

    template <typename T>
    void stack<T>::push(T&& n) {
        auto new_node = new ListNode<T>(std::move(n));
        new_node->next_ = top_;
        top_ = new_node;
    }

    template <typename T>
    void stack<T>::pop() {
        if (!this->empty()) {
            ListNode<T>* to_delete = top_;
            top_ = top_->next_;
            delete to_delete;
        }
    }

    template <typename T>
    T& stack<T>::top() const {
        return top_->data_;
    }

    template <typename T>
    bool stack<T>::empty() const {
        return top_ == nullptr;
    }

    template <typename T>
    uint64_t stack<T>::size() const {
        return std::distance(begin(), end());
    }

    template <typename T>
    stack<T> stack<T>::reversed() const {
        stack<T> reversed_stack;
        for (const T& o_item : *this) {
            reversed_stack.push(o_item);
        }
        return reversed_stack;
    }

    template <typename T>
    std::ostream& operator<<(std::ostream& out, const stack<T>& stack) {
        for (const T& item : stack) {
            out << item << " ";
        }
        return out;
    }
} // namespace rzt

//TODO: implement inner iterator&const iterator using either std::conditional or <Bool>
