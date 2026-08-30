#ifndef LANGUAGE_CPP_MINIVECTOR_H
#define LANGUAGE_CPP_MINIVECTOR_H

#include <cstddef>
#include <initializer_list>
#include <ranges>
#include <stdexcept>
#include <utility>
#include <memory>

template <typename T>
class MiniVector
{
public:
    MiniVector() noexcept
    {
        size_ = 0;
        capacity_ = 0;
        data_ = nullptr;
    };

    explicit MiniVector(const std::size_t count, const T& value = T())
    {
        size_ = count;
        capacity_ = count;
        data_ = static_cast<T*>(::operator new(sizeof(T) * capacity_));
        for (std::size_t i = 0; i < size_; i++)
        {
            std::construct_at(data_ + i, value);
        }
    }

    MiniVector(std::initializer_list<T> list)
    {
        size_ = list.size();
        capacity_ = list.size();
        data_ = static_cast<T*>(::operator new(sizeof(T) * capacity_));
        for (const auto& [i, object] : list | std::views::enumerate)
        {
            std::construct_at(data_ + i, object);
        }
    }

    ~MiniVector()
    {
        clear();
        ::operator delete(data_);
    }

    // Copie
    MiniVector(const MiniVector& other)
    {
        size_ = other.size_;
        capacity_ = other.capacity_;
        data_ = static_cast<T*>(::operator new(sizeof(T) * capacity_));
        for (std::size_t i = 0; i < size_; i++)
        {
            std::construct_at(data_ + i, other[i]);
        }
    }

    MiniVector& operator=(const MiniVector& other)
    {
        if (this == &other)
            return *this;

        clear();
        ::operator delete(data_);

        size_ = other.size_;
        capacity_ = other.capacity_;
        data_ = static_cast<T*>(::operator new(sizeof(T) * capacity_));

        for (std::size_t i = 0; i < size_; ++i)
        {
            std::construct_at(data_ + i, other.data_[i]);
        }

        return *this;
    }

    // Déplacement
    MiniVector(MiniVector&& other) noexcept
    {
        size_ = other.size_;
        capacity_ = other.capacity_;
        data_ = other.data_;
        other.data_ = nullptr;
        other.capacity_ = 0;
        other.size_ = 0;
    }

    MiniVector& operator=(MiniVector&& other) noexcept
    {
        if (this == &other)
            return *this;

        MiniVector tmp;
        swap(other);
        swap(tmp, other);

        return *this;
    }

    // Accès
    T& operator[](std::size_t index)
    {
        return data_[index];
    }

    const T& operator[](std::size_t index) const
    {
        return data_[index];
    }

    T& at(std::size_t index)
    {
        if (index >= size_)
        {
            throw std::out_of_range("index out of range");
        }
        return data_[index];
    }

    const T& at(std::size_t index) const
    {
        if (index >= size_)
        {
            throw std::out_of_range("index out of range");
        }
        return data_[index];
    }

    // Capacité
    [[nodiscard]] std::size_t size() const noexcept
    {
        return size_;
    }

    [[nodiscard]] std::size_t capacity() const noexcept
    {
        return capacity_;
    }

    [[nodiscard]] bool empty() const noexcept
    {
        return size_ == 0;
    }

    void reserve(const std::size_t new_capacity)
    {
        if (new_capacity > capacity_)
            reallocate(new_capacity);
    }

    // Modification
    void push_back(const T& value)
    {
        if (size_ == capacity_)
        {
            reallocate(size_ == 0 ? 1 : capacity_ * 2);
        }
        std::construct_at(data_ + size_, value);
        ++size_;
    }

    void push_back(T&& value)
    {
        if (size_ == capacity_)
        {
            reallocate(size_ == 0 ? 1 : capacity_ * 2);
        }
        std::construct_at(data_ + size_, std::move(value));
        ++size_;
    }

    void pop_back()
    {
        if (size_ == 0)
        {
            throw std::out_of_range("empty vector");
        }
        --size_;
        std::destroy_at(data_ + size_);
    }

    void clear() noexcept
    {
        for (std::size_t i = 0; i < size_; ++i)
        {
            std::destroy_at(data_ + i);
        }
        size_ = 0;
    }

    // Itérateurs simples
    T* begin() noexcept
    {
        return data_;
    }

    const T* begin() const noexcept
    {
        return data_;
    }

    T* end() noexcept
    {
        return  data_ ?  data_ + size_ : nullptr;
    }

    const T* end() const noexcept
    {
        return data_ ? data_ + size_ : nullptr;
    }

    static void swap(MiniVector& a, MiniVector& b) noexcept {
        std::swap(a.data_, b.data_);
        std::swap(a.size_, b.size_);
        std::swap(a.capacity_, b.capacity_);
    }

private:
    T* data_;
    std::size_t size_;
    std::size_t capacity_;

    void reallocate(const std::size_t new_capacity)
    {
        if (new_capacity <= capacity_)
            return;

        T* new_data = static_cast<T*>(::operator new(sizeof(T) * new_capacity));

        for (std::size_t i = 0; i < size_; ++i)
        {
            std::construct_at(new_data + i, std::move(data_[i]));
            std::destroy_at(data_ + i);
        }

        ::operator delete(data_);

        data_ = new_data;
        capacity_ = new_capacity;
    }

    void swap(MiniVector& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }
};


#endif //LANGUAGE_CPP_MINIVECTOR_H
