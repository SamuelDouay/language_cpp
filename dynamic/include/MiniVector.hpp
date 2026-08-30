#ifndef LANGUAGE_CPP_MINIVECTOR_H
#define LANGUAGE_CPP_MINIVECTOR_H

#include <cstddef>
#include <initializer_list>
#include <ranges>
#include <stdexcept>
#include <utility>
#include <memory>

template<typename T>

class MiniVector {
public:
    using value_type = T;
    using size_type = std::size_t;
    using iterator = T *;
    using const_iterator = const T *;

    MiniVector() noexcept {
        size_ = 0;
        capacity_ = 0;
        data_ = nullptr;
    }

    explicit MiniVector(const std::size_t count, const T &value = T()) : data_(nullptr), size_(0), capacity_(count) {
        if (count == 0)
            return;

        data_ = static_cast<T *>(::operator new(sizeof(T) * capacity_));

        try {
            for (; size_ < count; ++size_) {
                std::construct_at(data_ + size_, value);
            }
        } catch (...) {
            clear();
            ::operator delete(data_);
            throw;
        }
    }

    MiniVector(std::initializer_list<T> list) : data_(nullptr), size_(list.size()), capacity_(list.size()) {
        if (size_ == 0)
            return;

        data_ = static_cast<T *>(::operator new(sizeof(T) * capacity_));
        std::size_t constructed = 0;
        try {
            for (const auto &object: list) {
                std::construct_at(data_ + constructed, object);
                ++constructed;
            }
        } catch (...) {
            for (std::size_t j = 0; j < constructed; ++j)
                std::destroy_at(data_ + j);
            ::operator delete(data_);
            throw;
        }
    }

    ~MiniVector() {
        clear();
        ::operator delete(data_);
    }

    // Copie
    MiniVector(const MiniVector &other) : data_(nullptr), size_(other.size_), capacity_(other.capacity_) {
        if (other.capacity_ == 0)
            return;

        data_ = static_cast<T *>(::operator new(sizeof(T) * capacity_));
        std::size_t constructed = 0;
        try {
            for (; constructed < size_; ++constructed) {
                std::construct_at(data_ + constructed, other[constructed]);
            }
        } catch (...) {
            for (std::size_t j = 0; j < constructed; ++j)
                std::destroy_at(data_ + j);
            ::operator delete(data_);
            throw;
        }
    }

    MiniVector &operator=(const MiniVector &other) {
        if (this == &other)
            return *this;

        MiniVector copy(other);
        swap(copy);

        return *this;
    }

    // Déplacement
    MiniVector(MiniVector &&other) noexcept {
        size_ = other.size_;
        capacity_ = other.capacity_;
        data_ = other.data_;
        other.data_ = nullptr;
        other.capacity_ = 0;
        other.size_ = 0;
    }

    MiniVector &operator=(MiniVector &&other) noexcept {
        if (this == &other)
            return *this;

        clear();
        ::operator delete(data_);

        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;

        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;

        return *this;
    }

    // Accès
    T &operator[](std::size_t index) {
        return data_[index];
    }

    const T &operator[](std::size_t index) const {
        return data_[index];
    }

    T &at(std::size_t index) {
        if (index >= size_) {
            throw std::out_of_range("index out of range");
        }
        return data_[index];
    }

    const T &at(std::size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("index out of range");
        }
        return data_[index];
    }

    T &front() {
        if (empty()) throw std::out_of_range("empty vector");
        return data_[0];
    }

    const T &front() const {
        if (empty()) throw std::out_of_range("empty vector");
        return data_[0];
    }

    T &back() {
        if (empty()) throw std::out_of_range("empty vector");
        return data_[size_ - 1];
    }

    const T &back() const {
        if (empty()) throw std::out_of_range("empty vector");
        return data_[size_ - 1];
    }

    T *data() {
        return data_;
    }

    const T *data() const {
        return data_;
    }

    // Capacité
    [[nodiscard]] std::size_t size() const noexcept {
        return size_;
    }

    [[nodiscard]] std::size_t capacity() const noexcept {
        return capacity_;
    }

    [[nodiscard]] bool empty() const noexcept {
        return size_ == 0;
    }

    void reserve(const std::size_t new_capacity) {
        if (new_capacity > capacity_)
            reallocate(new_capacity);
    }

    // Modification
    void push_back(const T &value) {
        if (size_ == capacity_) {
            reallocate(size_ == 0 ? 1 : capacity_ * 2);
        }
        std::construct_at(data_ + size_, value);
        ++size_;
    }

    void push_back(T &&value) {
        if (size_ == capacity_) {
            reallocate(size_ == 0 ? 1 : capacity_ * 2);
        }
        std::construct_at(data_ + size_, std::move(value));
        ++size_;
    }

    void pop_back() {
        if (size_ == 0) {
            throw std::out_of_range("empty vector");
        }
        --size_;
        std::destroy_at(data_ + size_);
    }

    void clear() noexcept {
        for (std::size_t i = 0; i < size_; ++i) {
            std::destroy_at(data_ + i);
        }
        size_ = 0;
    }

    template<typename... Arg>
    T &emplace_back(Arg &&... arg) {
        if (size_ == capacity_) {
            reallocate(size_ == 0 ? 1 : capacity_ * 2);
        }
        std::construct_at(data_ + size_, std::forward<Arg>(arg)...);
        ++size_;
        return data_[size_ - 1];
    }

    void swap(MiniVector &other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

    // Itérateurs simples
    iterator begin() noexcept {
        return data_;
    }

    const_iterator begin() const noexcept {
        return data_;
    }

    iterator end() noexcept {
        return data_ ? data_ + size_ : nullptr;
    }

    const_iterator end() const noexcept {
        return data_ ? data_ + size_ : nullptr;
    }

private:
    T *data_;
    std::size_t size_;
    std::size_t capacity_;

    void reallocate(const std::size_t new_capacity) {
        if (new_capacity <= capacity_)
            return;

        T *new_data = static_cast<T *>(::operator new(sizeof(T) * new_capacity));

        std::size_t constructed = 0;

        try {
            for (; constructed < size_; ++constructed) {
                std::construct_at(new_data + constructed, std::move_if_noexcept(data_[constructed]));
            }
        } catch (...) {
            for (std::size_t i = 0; i < constructed; ++i) {
                std::destroy_at(new_data + i);
            }
            ::operator delete(new_data);
            throw;
        }
        for (std::size_t i = 0; i < size_; ++i) {
            std::destroy_at(data_ + i);
        }
        ::operator delete(data_);
        data_ = new_data;
        capacity_ = new_capacity;
    }
};


#endif //LANGUAGE_CPP_MINIVECTOR_H
