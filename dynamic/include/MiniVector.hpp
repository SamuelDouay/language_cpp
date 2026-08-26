#ifndef LANGUAGE_CPP_MINIVECTOR_H
#define LANGUAGE_CPP_MINIVECTOR_H

#include <cstddef>
#include <initializer_list>

#include <memory>

template <typename T>
class MiniVector
{
public:
    // Constructeurs / destructeur
    MiniVector(); // vecteur vide
    MiniVector(std::size_t count, const T& value = T());
    MiniVector(std::initializer_list<T> list);
    ~MiniVector();

    // Copie
    MiniVector(const MiniVector& other);
    MiniVector& operator=(const MiniVector& other);

    // Déplacement
    MiniVector(MiniVector&& other) noexcept;
    MiniVector& operator=(MiniVector&& other) noexcept;

    // Accès
    T& operator[](std::size_t index);
    const T& operator[](std::size_t index) const;
    T& at(std::size_t index);
    const T& at(std::size_t index) const;

    // Capacité
    std::size_t size() const noexcept;
    std::size_t capacity() const noexcept;
    bool empty() const noexcept;
    void reserve(std::size_t new_capacity);

    // Modification
    void push_back(const T& value);
    void push_back(T&& value);
    void pop_back();
    void clear() noexcept;

    // Itérateurs simples
    T* begin() noexcept;
    const T* begin() const noexcept;
    T* end() noexcept;
    const T* end() const noexcept;

private:
    T* data_;
    std::size_t size_;
    std::size_t capacity_;

    void reallocate(std::size_t new_capacity);
};


#endif //LANGUAGE_CPP_MINIVECTOR_H
