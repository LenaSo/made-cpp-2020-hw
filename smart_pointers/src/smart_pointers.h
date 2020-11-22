#pragma once


namespace task {

template <class T>
class UniquePtr {
private:
    T* ptr_;
public:
    using pointer = T*;
    using element_type = T;

    UniquePtr(const UniquePtr<element_type>& other) = delete;
    UniquePtr& operator=(const UniquePtr<element_type>& other) = delete;

    UniquePtr(pointer ptr);
    UniquePtr(UniquePtr<element_type>&& other);
    UniquePtr& operator = (UniquePtr<element_type>&& other);
    ~UniquePtr();

    //T& operator * ();
    typename std::add_lvalue_reference<T>::type operator*() const;
    pointer operator->() const noexcept;

    pointer get() const noexcept;
    pointer release() noexcept;
    void reset( pointer ptr = pointer() ) noexcept;
    void swap(UniquePtr& other) noexcept;
};

template <class T>
class WeakPtr;

template <class T>
class SharedPtr {
private:
    struct ControlBlock {
        T* ptr;
        size_t cnt;
    };
    ControlBlock* cb_;
    void release();
public:
    friend class WeakPtr<T>;

    using element_type = T;

    constexpr SharedPtr() noexcept;
    SharedPtr(const WeakPtr<T>& r);
    SharedPtr(T* ptr);
    SharedPtr(const SharedPtr<element_type>& other);
    SharedPtr(SharedPtr<element_type>&& other);
    SharedPtr& operator=(const SharedPtr<element_type>& other);
    SharedPtr& operator = (SharedPtr<element_type>&& other);
    ~SharedPtr();

    T& operator*() const noexcept;
    T* operator->() const noexcept;

    element_type* get() const noexcept;
    long use_count() const noexcept;
    void reset() noexcept;
    void reset(T*) noexcept;
    void swap(SharedPtr<T>& other) noexcept;
};

template <class T>
class WeakPtr {
private:
    struct ControlBlock {
        typename SharedPtr<T>::ControlBlock* const & sharedCb_;
        size_t cnt;
    };
    ControlBlock* cb_;
    void release();

public:
    using element_type = T;

    constexpr WeakPtr() noexcept;
    WeakPtr(const SharedPtr<T>& r) noexcept;
    WeakPtr(const WeakPtr<T>& r) noexcept;
    WeakPtr(WeakPtr<T>&& r) noexcept;
    ~WeakPtr();

    WeakPtr& operator=(const WeakPtr<T>& r) noexcept;
    WeakPtr& operator=(const SharedPtr<T>& r) noexcept;
    WeakPtr& operator=(WeakPtr<T>&& r) noexcept;

    bool expired() const noexcept;
    long use_count() const noexcept;
    SharedPtr<T> lock() const noexcept;
    void reset() noexcept;
    void swap(WeakPtr<T>& other) noexcept;

};

}  // namespace task


#include "smart_pointers.tpp"
