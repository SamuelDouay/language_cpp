#ifndef LANGUAGE_CPP_MINIVECTOR_H
#define LANGUAGE_CPP_MINIVECTOR_H

#include <cstddef>
#include <initializer_list>
#include <limits>
#include <stdexcept>
#include <utility>
#include <memory>
#include <iterator>
#include <algorithm>
#include <new>

template <typename T>

class MiniVector
{
public:
    using value_type = T;
    using size_type = std::size_t;
    using iterator = T*;
    using const_iterator = const T*;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    MiniVector() noexcept : data_(nullptr), size_(0), capacity_(0)
    {
    }

    explicit MiniVector(const size_type count, const T& value = T()) : data_(nullptr), size_(0), capacity_(count)
    {
        if (count == 0)
        {
            return;
        }

        data_ = static_cast<T*>(::operator new(sizeof(T) * capacity_));

        try
        {
            for (; size_ < count; ++size_)
            {
                std::construct_at(data_ + size_, value);
            }
        }
        catch (...)
        {
            clear();
            ::operator delete(data_);
            throw;
        }
    }

    MiniVector(std::initializer_list<T> list) : data_(nullptr), size_(0), capacity_(list.size())
    {
        if (capacity_ == 0)
        {
            return;
        }

        data_ = static_cast<T*>(::operator new(sizeof(T) * capacity_));
        try
        {
            for (const auto& object : list)
            {
                std::construct_at(data_ + size_, object);
                size_++;
            }
        }
        catch (...)
        {
            clear();
            ::operator delete(data_);
            throw;
        }
    }

    ~MiniVector()
    {
        clear();
        ::operator delete(data_);
    }

    template <std::input_iterator InputIt>
    MiniVector(InputIt first, InputIt last) : data_(nullptr), size_(0), capacity_(0)
    {
        MiniVector<T> tmp;
        for (; first != last; ++first)
        {
            tmp.push_back(*first);
        }
        swap(tmp);
    }

    // Copie
    MiniVector(const MiniVector& other) : data_(nullptr), size_(other.size_), capacity_(other.capacity_)
    {
        if (other.capacity_ == 0)
        {
            return;
        }

        data_ = static_cast<T*>(::operator new(sizeof(T) * capacity_));
        size_type constructed = 0;
        try
        {
            for (; constructed < size_; ++constructed)
            {
                std::construct_at(data_ + constructed, other[constructed]);
            }
        }
        catch (...)
        {
            for (size_type j = 0; j < constructed; ++j)
            {
                std::destroy_at(data_ + j);
            }
            ::operator delete(data_);
            throw;
        }
    }

    MiniVector& operator=(const MiniVector& other)
    {
        if (this == &other)
        {
            return *this;
        }

        MiniVector copy(other);
        swap(copy);

        return *this;
    }

    // Déplacement
    MiniVector(MiniVector&& other) noexcept : data_(other.data_), size_(other.size_), capacity_(other.capacity_)
    {
        other.data_ = nullptr;
        other.capacity_ = 0;
        other.size_ = 0;
    }

    MiniVector& operator=(MiniVector&& other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }

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
    reference operator[](size_type index)
    {
        return data_[index];
    }

    const_reference operator[](size_type index) const
    {
        return data_[index];
    }

    reference at(size_type index)
    {
        if (index >= size_)
        {
            throw std::out_of_range("index out of range");
        }
        return data_[index];
    }

    const_reference at(size_type index) const
    {
        if (index >= size_)
        {
            throw std::out_of_range("index out of range");
        }
        return data_[index];
    }

    reference front()
    {
        if (empty())
        {
            throw std::out_of_range("empty vector");
        }
        return data_[0];
    }

    const_reference front() const
    {
        if (empty())
        {
            throw std::out_of_range("empty vector");
        }
        return data_[0];
    }

    reference back()
    {
        if (empty())
        {
            throw std::out_of_range("empty vector");
        }
        return data_[size_ - 1];
    }

    const_reference back() const
    {
        if (empty())
        {
            throw std::out_of_range("empty vector");
        }
        return data_[size_ - 1];
    }

    T* data()
    {
        return data_;
    }

    const T* data() const
    {
        return data_;
    }

    // Capacité
    [[nodiscard]] size_type size() const noexcept
    {
        return size_;
    }

    [[nodiscard]] size_type capacity() const noexcept
    {
        return capacity_;
    }

    [[nodiscard]] bool empty() const noexcept
    {
        return size_ == 0;
    }

    [[nodiscard]] size_type max_size() const noexcept
    {
        return std::numeric_limits<size_type>::max() / sizeof(T);
    }

    [[nodiscard]] size_type growth_capacity() const
    {
        if (capacity_ == 0)
        {
            return 1;
        }

        if (capacity_ == max_size())
        {
            throw std::length_error("growth capacity exceeded");
        }

        if (capacity_ > max_size() / 2)
        {
            return max_size();
        }

        return capacity_ * 2;
    }

    void reserve(const size_type new_capacity)
    {
        if (new_capacity > max_size())
        {
            throw std::length_error("reserve capacity exceeded");
        }

        if (new_capacity <= capacity_)
        {
            return;
        }

        reallocate(new_capacity);
    }

    // Modification
    void push_back(const T& value)
    {
        if (size_ == capacity_)
        {
            T temp(value);
            reallocate(growth_capacity());
            std::construct_at(data_ + size_, std::move(temp));
        }
        else
        {
            std::construct_at(data_ + size_, value);
        }
        ++size_;
    }

    void push_back(T&& value)
    {
        if (size_ == capacity_)
        {
            T temp(std::move(value));
            reallocate(growth_capacity());
            std::construct_at(data_ + size_, std::move(temp));
        }
        else
        {
            std::construct_at(data_ + size_, std::move(value));
        }
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
        for (size_type i = 0; i < size_; ++i)
        {
            std::destroy_at(data_ + i);
        }
        size_ = 0;
    }

    void resize(size_type new_size)
    {
        if (size_ == new_size)
        {
            return;
        }

        if (size_ > new_size)
        {
            for (size_type i = new_size; i < size_; ++i)
            {
                std::destroy_at(data_ + i);
            }
            size_ = new_size;
            return;
        }

        reserve(new_size);
        size_type old_size = size_;
        try
        {
            for (; size_ < new_size; ++size_)
            {
                std::construct_at(data_ + size_);
            }
        }
        catch (...)
        {
            for (size_type i = old_size; i < size_; ++i)
            {
                std::destroy_at(data_ + i);
            }
            size_ = old_size;
            throw;
        }
    }

    void resize(size_type new_size, const T& value)
    {
        if (size_ == new_size)
        {
            return;
        }

        if (size_ > new_size)
        {
            for (size_type i = new_size; i < size_; ++i)
            {
                std::destroy_at(data_ + i);
            }
            size_ = new_size;
            return;
        }

        reserve(new_size);
        size_type old_size = size_;
        try
        {
            for (; size_ < new_size; ++size_)
            {
                std::construct_at(data_ + size_, value);
            }
        }
        catch (...)
        {
            for (size_type i = old_size; i < size_; ++i)
            {
                std::destroy_at(data_ + i);
            }
            size_ = old_size;
            throw;
        }
    }

    template <typename... Arg>
    reference emplace_back(Arg&&... arg)
    {
        if (size_ == capacity_)
        {
            T temp(std::forward<Arg>(arg)...);
            reallocate(growth_capacity());
            std::construct_at(data_ + size_, std::move(temp));
        }
        else
        {
            std::construct_at(data_ + size_, std::forward<Arg>(arg)...);
        }
        ++size_;
        return data_[size_ - 1];
    }

    void swap(MiniVector& other) noexcept
    {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

    void assign(size_type count, const T& value)
    {
        T local(value);
        clear();
        reserve(count);

        size_type old_size = size_;
        try
        {
            for (; size_ < count; ++size_)
            {
                std::construct_at(data_ + size_, local);
            }
        }
        catch (...)
        {
            for (size_type i = old_size; i < size_; ++i)
            {
                std::destroy_at(data_ + i);
            }
            size_ = old_size;
            throw;
        }
    }

    void assign(std::initializer_list<T> list)
    {
        clear();
        reserve(list.size());

        size_type old_size = size_;
        try
        {
            for (auto& item : list)
            {
                std::construct_at(data_ + size_, item);
                size_++;
            }
        }
        catch (...)
        {
            for (size_type i = old_size; i < size_; ++i)
            {
                std::destroy_at(data_ + i);
            }
            size_ = old_size;
            throw;
        }
    }

    template <typename InputIt>
        requires std::input_iterator<InputIt>
    void assign(InputIt first, InputIt last)
    {
        MiniVector<T> tmp;
        for (; first != last; ++first)
        {
            tmp.push_back(*first);
        }
        swap(tmp);
    }

    iterator erase(const_iterator pos)
    {
        std::size_t index = pos - cbegin();
        for (size_type i = index; i + 1 < size_; ++i)
        {
            data_[i] = std::move(data_[i + 1]);
        }
        --size_;
        std::destroy_at(data_ + size_);
        return data_ + index;
    }

    iterator erase(const_iterator first, const_iterator last)
    {
        std::size_t idx_first = first - cbegin();
        std::size_t idx_last = last - cbegin();
        std::size_t count = idx_last - idx_first;

        if (count == 0)
        {
            return data_ + idx_first;
        }
        for (std::size_t i = idx_first; i + count < size_; ++i)
        {
            data_[i] = std::move(data_[i + count]);
        }

        for (std::size_t i = 0; i < count; ++i)
        {
            size_--;
            std::destroy_at(data_ + size_);
        }
        return data_ + idx_first;
    }

    void shrink_to_fit()
    {
        if (capacity_ == size_)
        {
            return;
        }
        if (size_ == 0)
        {
            ::operator delete(data_);
            data_ = nullptr;
            capacity_ = 0;
            return;
        }

        T* new_data = static_cast<T*>(::operator new(sizeof(T) * size_));

        size_type constructed = 0;
        try
        {
            for (; constructed < size_; ++constructed)
            {
                std::construct_at(new_data + constructed, std::move_if_noexcept(data_[constructed]));
            }
        }
        catch (...)
        {
            for (size_type i = 0; i < constructed; ++i)
            {
                std::destroy_at(new_data + i);
            }
            ::operator delete(new_data);
            throw;
        }

        for (std::size_t i = 0; i < size_; ++i)
        {
            std::destroy_at(data_ + i);
        }
        ::operator delete(data_);
        data_ = new_data;
        capacity_ = size_;
    }

    // Itérateurs simples
    iterator begin() noexcept
    {
        return data_;
    }

    const_iterator begin() const noexcept
    {
        return data_;
    }

    const_iterator cbegin() const noexcept
    {
        return data_;
    }

    iterator end() noexcept
    {
        return data_ ? data_ + size_ : nullptr;
    }

    const_iterator end() const noexcept
    {
        return data_ ? data_ + size_ : nullptr;
    }

    const_iterator cend() const noexcept
    {
        return data_ ? data_ + size_ : nullptr;
    }

    reverse_iterator rbegin() noexcept
    {
        return reverse_iterator(end());
    }

    const_reverse_iterator rbegin() const noexcept
    {
        return const_reverse_iterator(cend());
    }

    const_reverse_iterator crbegin() const noexcept
    {
        return const_reverse_iterator(cend());
    }

    reverse_iterator rend() noexcept
    {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rend() const noexcept
    {
        return const_reverse_iterator(cbegin());
    }

    const_reverse_iterator crend() const noexcept
    {
        return const_reverse_iterator(cbegin());
    }


    // compare
    bool operator==(const MiniVector& other) const
    {
        if (size_ != other.size_) return false;
        for (std::size_t i = 0; i < size_; ++i)
        {
            if (data_[i] != other.data_[i]) return false;
        }
        return true;
    }

    bool operator<(const MiniVector& other) const
    {
        std::size_t min_size = std::min(size_, other.size_);
        for (std::size_t i = 0; i < min_size; ++i)
        {
            if (data_[i] != other.data_[i])
            {
                return data_[i] < other.data_[i];
            }
        }
        if (size_ != other.size_) return size_ < other.size_;
        return false;
    }

    bool operator>(const MiniVector& other) const
    {
        return other < *this;
    }

    bool operator<=(const MiniVector& other) const
    {
        return !(other < *this);
    }

    bool operator>=(const MiniVector& other) const
    {
        return !(*this < other);
    }

private:
    T* data_;
    size_type size_;
    size_type capacity_;

    void reallocate(const size_type new_capacity)
    {
        if (new_capacity <= capacity_)
        {
            return;
        }

        T* new_data = static_cast<T*>(::operator new(sizeof(T) * new_capacity));

        size_type constructed = 0;

        try
        {
            for (; constructed < size_; ++constructed)
            {
                std::construct_at(new_data + constructed, std::move_if_noexcept(data_[constructed]));
            }
        }
        catch (...)
        {
            for (size_type i = 0; i < constructed; ++i)
            {
                std::destroy_at(new_data + i);
            }
            ::operator delete(new_data);
            throw;
        }
        for (size_type i = 0; i < size_; ++i)
        {
            std::destroy_at(data_ + i);
        }
        ::operator delete(data_);
        data_ = new_data;
        capacity_ = new_capacity;
    }
};


#endif //LANGUAGE_CPP_MINIVECTOR_H
