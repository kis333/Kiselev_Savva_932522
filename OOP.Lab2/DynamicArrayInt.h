#pragma once
#include <iostream>

class DynamicArray {
public:
    class iterator;
    class const_iterator;

    DynamicArray() = default;
    DynamicArray(const int* data, int length);
    DynamicArray(const DynamicArray& other);
    DynamicArray(DynamicArray&& other) noexcept;
    ~DynamicArray();

    DynamicArray& operator=(const DynamicArray& other);
    DynamicArray& operator=(DynamicArray&& other) noexcept;

    DynamicArray& operator+=(const int& value);
    DynamicArray& operator+=(const DynamicArray& other);
    DynamicArray operator+(const int& value) const;
    DynamicArray operator+(const DynamicArray& other) const;

    bool operator==(const DynamicArray& other) const;
    bool operator!=(const DynamicArray& other) const;

    int getLength() const;
    int getCapacity() const;
    int& operator[](int index);
    const int& operator[](int index) const;
    void swap(DynamicArray& other);
    int find(const int& value) const;
    void sort();
    bool insert(int index, const int& value);
    bool removeAt(int index);
    bool remove(const int& value);
    int removeAll(const int& value);
    int getMin() const;
    int getMax() const;

    iterator insert(iterator pos, const int& value);
    iterator erase(iterator pos);
    iterator erase(iterator first, iterator last);
    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;

private:
    int* data_ = nullptr;
    int length_ = 0;
    int capacity_ = 0;

    void reserve(int capacity);
    void shrink();
};

class DynamicArray::iterator {
public:
    iterator() = default;
    iterator(int* data, int index);
    bool operator==(const iterator& other) const;
    bool operator!=(const iterator& other) const;
    iterator& operator++();
    iterator operator++(int);
    int& operator*() const;

private:
    int* data_ = nullptr;
    int index_ = 0;
    friend class DynamicArray;
    friend class const_iterator;
};

class DynamicArray::const_iterator {
public:
    const_iterator() = default;
    const_iterator(const int* data, int index);
    const_iterator(const iterator& other);
    bool operator==(const const_iterator& other) const;
    bool operator!=(const const_iterator& other) const;
    const_iterator& operator++();
    const_iterator operator++(int);
    const int& operator*() const;

private:
    const int* data_ = nullptr;
    int index_ = 0;
};

std::ostream& operator<<(std::ostream& out, const DynamicArray& array);
std::istream& operator>>(std::istream& in, DynamicArray& array);
