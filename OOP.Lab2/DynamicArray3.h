#pragma once

#include <iostream>
#include <cassert>

template<typename ItemType>
class DynamicArray
{
public:
    class iterator;
    class const_iterator;

    DynamicArray() = default;
    DynamicArray(const ItemType* const arrayData, const int arrayLength);
    DynamicArray(const DynamicArray& other);
    DynamicArray(DynamicArray&& other) noexcept;
    ~DynamicArray();

    DynamicArray& operator=(const DynamicArray& other);
    DynamicArray& operator=(DynamicArray&& other) noexcept;

    DynamicArray& operator+=(const ItemType& item);
    DynamicArray& operator+=(const DynamicArray& other);
    DynamicArray operator+(const ItemType& item) const;
    DynamicArray operator+(const DynamicArray& other) const;

    bool operator==(const DynamicArray& other) const;
    bool operator!=(const DynamicArray& other) const;

    int getLength() const;
    int getCapacity() const;

    ItemType& operator[](const int index);
    const ItemType& operator[](const int index) const;

    void swap(DynamicArray& other);

    int find(const ItemType& item) const;
    void sort();
    bool insert(const int index, const ItemType& item);
    bool removeAt(const int index);
    bool remove(const ItemType& item);
    int removeAll(const ItemType& item);
    ItemType getMin() const;
    ItemType getMax() const;

    iterator insert(iterator position, const ItemType& item);
    iterator erase(iterator position);
    iterator erase(iterator first, iterator last);

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;

private:
    ItemType* arrayData_ = nullptr;
    int arrayLength_ = 0;
    int arrayCapacity_ = 0;

    void reserve(int newCapacity);
    void shrink();
};

template<typename ItemType>
class DynamicArray<ItemType>::iterator
{
public:
    iterator() = default;
    iterator(ItemType* arrayData, int elementIndex);

    bool operator!=(const iterator& otherIterator) const;
    bool operator==(const iterator& otherIterator) const;
    iterator& operator++();
    iterator operator++(int);
    ItemType& operator*() const;

private:
    ItemType* arrayData_ = nullptr;
    int elementIndex_ = 0;
    friend class DynamicArray<ItemType>;
};

template<typename ItemType>
class DynamicArray<ItemType>::const_iterator
{
public:
    const_iterator() = default;
    const_iterator(const ItemType* arrayData, int elementIndex);
    const_iterator(const iterator& other);

    bool operator!=(const const_iterator& otherIterator) const;
    bool operator==(const const_iterator& otherIterator) const;
    const_iterator& operator++();
    const_iterator operator++(int);
    const ItemType& operator*() const;

private:
    const ItemType* arrayData_ = nullptr;
    int elementIndex_ = 0;
};

template<typename ItemType>
std::ostream& operator<<(std::ostream& outputStream, const DynamicArray<ItemType>& arrayRef);

template<typename ItemType>
std::istream& operator>>(std::istream& inputStream, DynamicArray<ItemType>& arrayRef);

#include "DynamicArray3.inl"
