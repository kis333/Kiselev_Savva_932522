#pragma once

#include <assert.h>
#include <iostream>
#include <utility>

template<typename ItemType>
class DynamicArray
{
    public:

        class iterator;
        class const_iterator;

        DynamicArray() = default;
        DynamicArray(const ItemType* const arrayData, const int arrayLength);
        DynamicArray(const DynamicArray& other);
        DynamicArray(DynamicArray&& other);
        ~DynamicArray();

        DynamicArray& operator=(const DynamicArray& other);
        DynamicArray& operator=(DynamicArray&& other);

        DynamicArray& operator+=(const ItemType& item);
        DynamicArray& operator+=(const DynamicArray& other);

        int getLength() const;

        ItemType& operator[](const int index);
        const ItemType& operator[](const int index) const;

        void swap(DynamicArray& other);

        iterator begin();
        iterator end();
        const_iterator begin() const;
        const_iterator end() const;

        int find(const ItemType& item) const;
        ItemType& maxElement();
        ItemType& minElement();

        void sort();

        bool insert(const int index, const ItemType& item);
        bool erase(const int index);
        bool eraseByValue(const ItemType& item);
        int  eraseAll(const ItemType& item);

        iterator insert(iterator position, const ItemType& item);
        iterator erase(iterator position);
        iterator erase(iterator first, iterator last);

    private:

        ItemType *arrayData_ = nullptr;
        int arrayLength_ = 0;
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
        iterator& operator--();
        iterator operator--(int);
        ItemType& operator*();
        ItemType* operator->();
        iterator operator+(int offset) const;
        iterator operator-(int offset) const;
        int operator-(const iterator& other) const;

        ItemType* base() const { return arrayData_; }
        int index() const { return elementIndex_; }

    private:

        ItemType *arrayData_ = nullptr;
        int elementIndex_ = 0;
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
        const_iterator& operator--();
        const_iterator operator--(int);
        const ItemType& operator*() const;
        const ItemType* operator->() const;
        const_iterator operator+(int offset) const;
        const_iterator operator-(int offset) const;
        int operator-(const const_iterator& other) const;

    private:

        const ItemType *arrayData_ = nullptr;
        int elementIndex_ = 0;
};

template<typename ItemType>
DynamicArray<ItemType> operator+(const DynamicArray<ItemType>& array, const ItemType& item);

template<typename ItemType>
DynamicArray<ItemType> operator+(const DynamicArray<ItemType>& lhs, const DynamicArray<ItemType>& rhs);

template<typename ItemType>
bool operator==(const DynamicArray<ItemType>& lhs, const DynamicArray<ItemType>& rhs);

template<typename ItemType>
bool operator!=(const DynamicArray<ItemType>& lhs, const DynamicArray<ItemType>& rhs);

template<typename ItemType>
std::ostream& operator<<(std::ostream& outputStream, const DynamicArray<ItemType>& arrayRef);

#include "DynamicArray.inl"
