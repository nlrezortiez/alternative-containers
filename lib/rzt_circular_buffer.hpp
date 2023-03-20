#include "iterator/random_access_iterator.hpp"

template<typename T, typename Alloc>
class circular_buffer {
public:

    typedef size_t size_type;
    typedef size_t capacity_type;
    typedef Alloc allocator_type;
    typedef circular_buffer& reference;
    typedef const circular_buffer& const_reference;
    typedef circular_buffer_iterator::iterator iterator; //?? iterator ?? 
    typedef circular_buffer_iterator::const_iterator const_iterator;
    typedef circular_buffer_iterator::reverse_iterator reverse_iterator;
    typedef circular_buffer_iterator::const_reverse_iterator const_reverse_iterator;
    typedef typename Alloc::value_type value_type;
    typedef value_type& param_value_type;
    typedef value_type&& rvalue_type;
    // construct/copy/destruct
    explicit circular_buffer(const allocator_type& = allocator_type()) noexcept;
    explicit circular_buffer(capacity_type,  const allocator_type& = allocator_type());
    circular_buffer(size_type, param_value_type, const allocator_type & = allocator_type());
    circular_buffer(capacity_type, size_type, param_value_type, const allocator_type& = allocator_type());
    circular_buffer(const circular_buffer< T, Alloc > &);
    circular_buffer(circular_buffer< T, Alloc > &&) noexcept;

    template<typename InputIterator> 
    circular_buffer(InputIterator, InputIterator, const allocator_type& = allocator_type());

    template<typename InputIterator> 
    circular_buffer(capacity_type, InputIterator, InputIterator, const allocator_type& = allocator_type());
    
    circular_buffer<T, Alloc>& operator=(const circular_buffer<T, Alloc>&);
    circular_buffer<T, Alloc>& operator=(circular_buffer<T, Alloc>&&) noexcept;

    ~circular_buffer();

    // public member functions
    allocator_type get_allocator() const noexcept;
    allocator_type& get_allocator() noexcept;
    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;
    reverse_iterator rbegin() noexcept;
    reverse_iterator rend() noexcept;
    const_reverse_iterator rbegin() const noexcept;
    const_reverse_iterator rend() const noexcept;
    reference operator[](size_type);
    const_reference operator[](size_type) const;

    reference at(size_type);
    const_reference at(size_type) const;
    reference front();
    reference back();
    const_reference front() const;
    const_reference back() const;
    void rotate(const_iterator);
    size_type size() const noexcept;
    size_type max_size() const noexcept;
    bool empty() const noexcept;
    bool full() const noexcept;
    size_type reserve() const noexcept;
    capacity_type capacity() const noexcept;
    void set_capacity(capacity_type);
    void resize(size_type, param_value_type = value_type());
    void rset_capacity(capacity_type);
    void rresize(size_type, param_value_type = value_type());
    
    void assign(size_type, param_value_type);
    void assign(capacity_type, size_type, param_value_type);
    template<typename InputIterator> 
    void assign(InputIterator, InputIterator);
    template<typename InputIterator> 
    void assign(capacity_type, InputIterator, InputIterator);

    void swap(circular_buffer< T, Alloc > &) noexcept;
    void push_back(param_value_type);
    void push_back(rvalue_type);
    void push_back();
    void push_front(param_value_type);
    void push_front(rvalue_type);
    void push_front();
    void pop_back();
    void pop_front();
    
    iterator insert(iterator, param_value_type);
    iterator insert(iterator, rvalue_type);
    iterator insert(iterator);
    void insert(iterator, size_type, param_value_type);
    template<typename InputIterator> 
    void insert(iterator, InputIterator, InputIterator);
    iterator rinsert(iterator, param_value_type);
    iterator rinsert(iterator, rvalue_type);
    iterator rinsert(iterator);
    void rinsert(iterator, size_type, param_value_type);
    template<typename InputIterator> 
    void rinsert(iterator, InputIterator, InputIterator);
    
    iterator erase(iterator);
    iterator erase(iterator, iterator);
    iterator rerase(iterator);
    iterator rerase(iterator, iterator);
    void erase_begin(size_type);
    void erase_end(size_type);
    void clear() noexcept;

private:
    
};
