#pragma once
#include <memory>

template <typename T>
class ResourceArray {
    T* data_;
    size_t topIndex_;
    size_t capacity_;
public:
    explicit ResourceArray(size_t n)
        : topIndex_{0}
        , capacity_{n}
    {
        data_ = new T[n];
    }
    ResourceArray(const ResourceArray&) =delete;
    ResourceArray(ResourceArray&& other)
        : data_{other.data_}
        , capacity_{other.capacity_}
        , topIndex_{other.topIndex_}
    {
        other.data_ = nullptr;
    }
    ResourceArray& operator=(const ResourceArray&) = delete;
    ResourceArray& operator=(ResourceArray&&) =delete;

    ~ResourceArray() {
        delete[] data_;
    }

    void push(const T& elem) {
        data_[topIndex_++] = elem;
    }
    void push(T&& elem) {
        data_[topIndex_++] = std::move(elem);
    }

    size_t topIndex() {
        return topIndex_;
    }

    T& top() {
        ASSERT(topIndex_ > 0);
        return data_[topIndex_ - 1];
    }

    T& operator[] (int i) {
        ASSERT(i >= 0 && i < capacity_)
        return data_[i];
    }
};