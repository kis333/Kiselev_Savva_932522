#include "DynamicInt.h"

#include <algorithm>
#include <cassert>
#include <istream>
#include <ostream>
#include <utility>

DynamicArray::DynamicArray(const int* data, int length) {
    if (length < 0 || (length > 0 && data == nullptr)) return;
    length_ = capacity_ = length;
    if (capacity_) data_ = new int[capacity_];
    for (int i = 0; i < length_; ++i) data_[i] = data[i];
}

DynamicArray::DynamicArray(const DynamicArray& o) {
    length_ = o.length_;
    capacity_ = o.capacity_;
    if (capacity_) data_ = new int[capacity_];
    for (int i = 0; i < length_; ++i) data_[i] = o.data_[i];
}

DynamicArray::DynamicArray(DynamicArray&& o) noexcept
    : data_(o.data_), length_(o.length_), capacity_(o.capacity_) {
    o.data_ = nullptr;
    o.length_ = o.capacity_ = 0;
}

DynamicArray::~DynamicArray() { delete[] data_; }

DynamicArray& DynamicArray::operator=(const DynamicArray& o) {
    if (this != &o) {
        DynamicArray copy(o);
        swap(copy);
    }
    return *this;
}

DynamicArray& DynamicArray::operator=(DynamicArray&& o) noexcept {
    if (this != &o) {
        delete[] data_;
        data_ = o.data_;
        length_ = o.length_;
        capacity_ = o.capacity_;
        o.data_ = nullptr;
        o.length_ = o.capacity_ = 0;
    }
    return *this;
}

void DynamicArray::reserve(int c) {
    if (c <= capacity_) return;
    int* p = new int[c];
    for (int i = 0; i < length_; ++i) p[i] = data_[i];
    delete[] data_;
    data_ = p;
    capacity_ = c;
}

void DynamicArray::shrink() {
    if (capacity_ <= 4 || length_ > capacity_ / 4) return;
    int c = capacity_ / 2;
    if (c < 4) c = 4;
    if (c < length_) c = length_;
    int* p = new int[c];
    for (int i = 0; i < length_; ++i) p[i] = data_[i];
    delete[] data_;
    data_ = p;
    capacity_ = c;
}

DynamicArray& DynamicArray::operator+=(const int& v) {
    if (length_ == capacity_) reserve(capacity_ == 0 ? 4 : capacity_ * 2);
    data_[length_++] = v;
    return *this;
}

DynamicArray& DynamicArray::operator+=(const DynamicArray& o) {
    if (this == &o) {
        DynamicArray copy(o);
        return *this += copy;
    }
    int old = length_;
    int n = length_ + o.length_;
    if (n > capacity_) {
        int c = capacity_ ? capacity_ : 4;
        while (c < n) c *= 2;
        reserve(c);
    }
    for (int i = 0; i < o.length_; ++i) data_[old + i] = o.data_[i];
    length_ = n;
    return *this;
}

DynamicArray DynamicArray::operator+(const int& v) const {
    DynamicArray r(*this);
    r += v;
    return r;
}

DynamicArray DynamicArray::operator+(const DynamicArray& o) const {
    DynamicArray r(*this);
    r += o;
    return r;
}

bool DynamicArray::operator==(const DynamicArray& o) const {
    if (length_ != o.length_) return false;
    for (int i = 0; i < length_; ++i)
        if (data_[i] != o.data_[i]) return false;
    return true;
}

bool DynamicArray::operator!=(const DynamicArray& o) const {
    return !(*this == o);
}

int DynamicArray::getLength() const { return length_; }

int DynamicArray::getCapacity() const { return capacity_; }

int& DynamicArray::operator[](int i) {
    assert(i >= 0 && i < length_ && "Index out of range");
    return data_[i];
}

const int& DynamicArray::operator[](int i) const {
    assert(i >= 0 && i < length_ && "Index out of range");
    return data_[i];
}

void DynamicArray::swap(DynamicArray& o) {
    std::swap(data_, o.data_);
    std::swap(length_, o.length_);
    std::swap(capacity_, o.capacity_);
}

int DynamicArray::find(const int& v) const {
    for (int i = 0; i < length_; ++i)
        if (data_[i] == v) return i;
    return -1;
}

void DynamicArray::sort() {
    for (int i = 0; i < length_; ++i)
        for (int j = 0; j + 1 < length_ - i; ++j)
            if (data_[j] > data_[j + 1])
                std::swap(data_[j], data_[j + 1]);
}

bool DynamicArray::insert(int i, const int& v) {
    if (i < 0 || i > length_) return false;
    if (length_ == capacity_) reserve(capacity_ ? capacity_ * 2 : 4);
    for (int j = length_; j > i; --j) data_[j] = data_[j - 1];
    data_[i] = v;
    ++length_;
    return true;
}

bool DynamicArray::removeAt(int i) {
    if (i < 0 || i >= length_) return false;
    for (int j = i; j < length_ - 1; ++j) data_[j] = data_[j + 1];
    --length_;
    shrink();
    return true;
}

bool DynamicArray::remove(const int& v) {
    int i = find(v);
    return i != -1 && removeAt(i);
}

int DynamicArray::removeAll(const int& v) {
    int n = 0, i = 0;
    while (i < length_) {
        if (data_[i] == v) { removeAt(i); ++n; }
        else ++i;
    }
    return n;
}

int DynamicArray::getMin() const {
    assert(length_ > 0 && "Array is empty");
    int r = data_[0];
    for (int i = 1; i < length_; ++i)
        if (data_[i] < r) r = data_[i];
    return r;
}

int DynamicArray::getMax() const {
    assert(length_ > 0 && "Array is empty");
    int r = data_[0];
    for (int i = 1; i < length_; ++i)
        if (data_[i] > r) r = data_[i];
    return r;
}

DynamicArray::iterator DynamicArray::insert(iterator p, const int& v) {
    if (p.data_ != data_ || p.index_ < 0 || p.index_ > length_) return end();
    int i = p.index_;
    insert(i, v);
    return iterator(data_, i);
}

DynamicArray::iterator DynamicArray::erase(iterator p) {
    if (p.data_ != data_ || p.index_ < 0 || p.index_ >= length_) return end();
    int i = p.index_;
    removeAt(i);
    return iterator(data_, i);
}

DynamicArray::iterator DynamicArray::erase(iterator f, iterator l) {
    if (f.data_ != data_ || l.data_ != data_ || f.index_ < 0 ||
        l.index_ < f.index_ || l.index_ > length_) return end();
    int n = l.index_ - f.index_;
    for (int i = 0; i < n; ++i) removeAt(f.index_);
    return iterator(data_, f.index_);
}

DynamicArray::iterator DynamicArray::begin() { return iterator(data_, 0); }
DynamicArray::iterator DynamicArray::end() { return iterator(data_, length_); }

DynamicArray::const_iterator DynamicArray::begin() const { return const_iterator(data_, 0); }
DynamicArray::const_iterator DynamicArray::end()   const { return const_iterator(data_, length_); }

DynamicArray::const_iterator DynamicArray::cbegin() const { return const_iterator(data_, 0); }
DynamicArray::const_iterator DynamicArray::cend()   const { return const_iterator(data_, length_); }

DynamicArray::iterator::iterator(int* d, int i) : data_(d), index_(i) {}

bool DynamicArray::iterator::operator==(const iterator& o) const {
    return data_ == o.data_ && index_ == o.index_;
}

bool DynamicArray::iterator::operator!=(const iterator& o) const {
    return !(*this == o);
}

DynamicArray::iterator& DynamicArray::iterator::operator++() {
    ++index_;
    return *this;
}

DynamicArray::iterator DynamicArray::iterator::operator++(int) {
    iterator old(*this);
    ++index_;
    return old;
}

int& DynamicArray::iterator::operator*() const { return data_[index_]; }

DynamicArray::const_iterator::const_iterator(const int* d, int i) : data_(d), index_(i) {}

DynamicArray::const_iterator::const_iterator(const iterator& o)
    : data_(o.data_), index_(o.index_) {
}

bool DynamicArray::const_iterator::operator==(const const_iterator& o) const {
    return data_ == o.data_ && index_ == o.index_;
}

bool DynamicArray::const_iterator::operator!=(const const_iterator& o) const {
    return !(*this == o);
}

DynamicArray::const_iterator& DynamicArray::const_iterator::operator++() {
    ++index_;
    return *this;
}

DynamicArray::const_iterator DynamicArray::const_iterator::operator++(int) {
    const_iterator old(*this);
    ++index_;
    return old;
}

const int& DynamicArray::const_iterator::operator*() const { return data_[index_]; }

std::ostream& operator<<(std::ostream& out, const DynamicArray& a) {
    out << "[";
    for (int i = 0; i < a.getLength(); ++i) {
        out << a[i];
        if (i + 1 < a.getLength()) out << ", ";
    }
    out << "]";
    return out;
}

std::istream& operator>>(std::istream& in, DynamicArray& a) {
    int v;
    if (in >> v) a += v;
    return in;
}
