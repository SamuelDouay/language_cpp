#ifndef LANGUAGE_CPP_DYNAMICBITSET_H
#define LANGUAGE_CPP_DYNAMICBITSET_H

#include <cstddef>
#include <string>
#include <iosfwd>
#include <stdexcept>
#include "MiniVector.hpp"

class DynamicBitset
{
public:
    using size_type = std::size_t;

    // ==== Constructeurs / destructeur ====
    explicit DynamicBitset(size_type n = 0) : bytes_((n + 7) / 8, 0), bit_count_(n)
    {
    }

    DynamicBitset(size_type n, bool value) : bytes_((n + 7) / 8, value ? static_cast<unsigned char>(0xFF) : 0),
                                             bit_count_(value ? n : 0)
    {
        if (n > 0 && (n % 8 != 0))
        {
            size_type const last_byte = (n - 1) / 8;
            unsigned char const mask = static_cast<unsigned char>((1u << (n % 8)) - 1);
            bytes_[last_byte] &= mask;
        }
    }

    explicit DynamicBitset(const std::string& str) : bytes_((str.size() + 7) / 8), bit_count_(str.size())
    {
        for (size_type i = 0; i < str.size(); ++i)
        {
            if (str[i] != '1' && str[i] != '0')
            {
                throw std::invalid_argument("Invalid character");
            }

            std::size_t pos = bit_count_ - 1 - i;
            if (str[i] == '1')
            {
                set(pos);
            }
        }
    }

    DynamicBitset(const DynamicBitset&) = default;

    DynamicBitset& operator=(const DynamicBitset&) = default;

    DynamicBitset(DynamicBitset&&) noexcept = default;

    DynamicBitset& operator=(DynamicBitset&&) noexcept = default;

    ~DynamicBitset() = default;

    // ==== Capacité ====
    [[nodiscard]] size_type size() const noexcept
    {
        return bit_count_;
    }

    [[nodiscard]] bool empty() const noexcept
    {
        return bytes_.empty();
    }

    // ==== Opérations sur un bit ====
    DynamicBitset& set(size_type pos)
    {
        if (pos >= bit_count_)
        {
            throw std::out_of_range("out of range");
        }
        unsigned char const mask = 1u << pos % 8;
        bytes_.at(pos / 8) |= mask;
        return *this;
    }

    DynamicBitset& set(size_type pos, bool value)
    {
        if (pos >= bit_count_)
        {
            throw std::out_of_range("out of range");
        }
        if (value)
        {
            return set(pos);
        }
        return reset(pos);
    }

    DynamicBitset& reset(size_type pos)
    {
        if (pos >= bit_count_)
        {
            throw std::out_of_range("out of range");
        }

        unsigned char const mask = 1u << pos % 8;
        if (test(pos))
        {
            bytes_.at(pos / 8) &= ~mask;
        }
        return *this;
    }

    DynamicBitset& flip(size_type pos)
    {
        if (pos >= bit_count_)
        {
            throw std::out_of_range("out of range");
        }
        if (test(pos))
        {
            return reset(pos);
        }
        return set(pos);
    }

    [[nodiscard]] bool test(size_type pos) const
    {
        if (pos >= bit_count_)
        {
            throw std::out_of_range("out of range");
        }
        unsigned char const mask = 1u << pos % 8;
        return (bytes_.at(pos / 8) & mask) != 0;
    }

    // ==== Opérations sur tous les bits ====
    DynamicBitset& set() noexcept
    {
        for (size_type pos = 0; pos < bit_count_; ++pos)
        {
            set(pos);
        }
        return *this;
    }

    DynamicBitset& reset() noexcept
    {
        for (size_type pos = 0; pos < bit_count_; ++pos)
        {
            reset(pos);
        }
        return *this;
    }

    DynamicBitset& flip() noexcept
    {
        for (size_type pos = 0; pos < bit_count_; ++pos)
        {
            flip(pos);
        }
        return *this;
    }

    // ==== Requêtes globales ====
    [[nodiscard]] size_type count() const noexcept
    {
        size_type nb_bits = 0;

        for (size_type pos = 0; pos < bit_count_; ++pos)
        {
            if (test(pos))
            {
                nb_bits++;
            }
        }
        return nb_bits;
    }

    [[nodiscard]] bool all() const noexcept
    {
        if (bit_count_ == 0)
        {
            return true;
        }

        for (size_type pos = 0; pos < bit_count_; ++pos)
        {
            if (!test(pos))
            {
                return false;
            }
        }
        return true;
    }

    [[nodiscard]] bool any() const noexcept
    {
        for (size_type pos = 0; pos < bit_count_; ++pos)
        {
            if (test(pos))
            {
                return true;
            }
        }
        return false;
    }

    [[nodiscard]] bool none() const noexcept
    {
        for (size_type pos = 0; pos < bit_count_; ++pos)
        {
            if (test(pos))
            {
                return false;
            }
        }
        return true;
    }

    // ==== Comparaison ====
    [[nodiscard]] bool operator==(const DynamicBitset& other) const noexcept
    {
        if (size() != other.size())
        {
            return false;
        }
        return bytes_ == other.bytes_;
    }

    // ==== Opérateurs bit à bit (membres mutateurs) ====
    DynamicBitset& operator&=(const DynamicBitset& other)
    {
        if (other.bit_count_ != bit_count_)
        {
            throw std::invalid_argument("invalid other");
        }

        for (size_type pos = 0; pos < bytes_.size(); ++pos)
        {
            bytes_.at(pos) &= other.bytes_.at(pos);
        }
        return *this;
    }

    DynamicBitset& operator|=(const DynamicBitset& other)
    {
        if (other.bit_count_ != bit_count_)
        {
            throw std::invalid_argument("invalid other");
        }

        for (size_type pos = 0; pos < bytes_.size(); ++pos)
        {
            bytes_.at(pos) |= other.bytes_.at(pos);
        }
        return *this;
    }

    DynamicBitset& operator^=(const DynamicBitset& other)
    {
        if (other.bit_count_ != bit_count_)
        {
            throw std::invalid_argument("invalid other");
        }

        for (size_type pos = 0; pos < bytes_.size(); ++pos)
        {
            bytes_.at(pos) ^= other.bytes_.at(pos);
        }
        return *this;
    }

    // ==== Complément (non-mutant) ====
    [[nodiscard]] DynamicBitset operator~() const
    {
        DynamicBitset copy(*this);
        copy.flip();
        if ((bit_count_ % 8 != 0))
        {
            size_type const last_byte = (bit_count_ - 1) / 8;
            unsigned char const mask = static_cast<unsigned char>((1u << (bit_count_ % 8)) - 1);
            copy.bytes_.at(last_byte) &= mask;
        }
        return copy;
    }

    // ==== Décalages (membres mutateurs) ====
    DynamicBitset& operator<<=(size_type n)
    {
        if (n == 0)
        {
            return *this;
        }
        if (n >= bit_count_)
        {
            return reset();
        }

        for (size_type pos = bit_count_ - 1; pos >= n; --pos)
        {
            set(pos, test(pos - n));
        }

        for (size_type pos = 0; pos <= n - 1; ++pos)
        {
            reset(pos);
        }

        return *this;
    }

    DynamicBitset& operator>>=(size_type n)
    {
        if (n == 0)
        {
            return *this;
        }
        if (n >= bit_count_)
        {
            return reset();
        }

        for (size_type pos = 0; pos < bit_count_ - n; ++pos)
        {
            set(pos, test(pos + n));
        }

        for (size_type pos = bit_count_ - n; pos < bit_count_; ++pos)
        {
            reset(pos);
        }

        return *this;
    }

    // ==== Conversion ====
    [[nodiscard]] std::string to_string() const
    {
        std::string result;
        result.reserve(bit_count_);
        for (size_type pos = bit_count_; pos-- > 0;)
        {
            result.push_back(test(pos) ? '1' : '0');
        }
        return result;
    }

    // ==== Swap ====
    void swap(DynamicBitset& other) noexcept
    {
        bytes_.swap(other.bytes_);
        std::swap(bit_count_, other.bit_count_);
    }

private:
    MiniVector<unsigned char> bytes_;
    size_type bit_count_;
};

// ==== Opérateurs libres ====
inline DynamicBitset operator&(DynamicBitset lhs, const DynamicBitset& rhs)
{
    return lhs &= rhs;
}

inline DynamicBitset operator|(DynamicBitset lhs, const DynamicBitset& rhs)
{
    return lhs |= rhs;
}

inline DynamicBitset operator^(DynamicBitset lhs, const DynamicBitset& rhs)
{
    return lhs ^= rhs;
}

inline DynamicBitset operator<<(DynamicBitset bs, std::size_t n)
{
    return bs <<= n;
}

inline DynamicBitset operator>>(DynamicBitset bs, std::size_t n)
{
    return bs >>= n;
}

inline void swap(DynamicBitset& a, DynamicBitset& b) noexcept
{
    a.swap(b);
}

inline std::ostream& operator<<(std::ostream& os, const DynamicBitset& bs)
{
    return os << bs.to_string();
}

#endif
