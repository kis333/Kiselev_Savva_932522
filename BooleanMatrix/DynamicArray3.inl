#pragma once

#include <algorithm>
#include <utility>

template<typename ItemType>
DynamicArray<ItemType>::DynamicArray(const ItemType* const arrayData, const int arrayLength)
{
    if (arrayLength < 0)
        return;

    if (arrayLength > 0 && arrayData == nullptr)
        return;

    arrayLength_ = arrayLength;
    arrayCapacity_ = arrayLength;
    if (arrayCapacity_ > 0)
        arrayData_ = new ItemType[arrayCapacity_];

    for (int index = 0; index < arrayLength_; index++)
        arrayData_[index] = arrayData[index];
}

template<typename ItemType>
DynamicArray<ItemType>::DynamicArray(const DynamicArray& other)
{
    arrayLength_ = other.arrayLength_;
    arrayCapacity_ = other.arrayCapacity_;

    if (arrayCapacity_ > 0)
        arrayData_ = new ItemType[arrayCapacity_];

    for (int index = 0; index < arrayLength_; index++)
        arrayData_[index] = other.arrayData_[index];
}

template<typename ItemType>
DynamicArray<ItemType>::DynamicArray(DynamicArray&& other) noexcept
{
    arrayData_ = other.arrayData_;
    arrayLength_ = other.arrayLength_;
    arrayCapacity_ = other.arrayCapacity_;

    other.arrayData_ = nullptr;
    other.arrayLength_ = 0;
    other.arrayCapacity_ = 0;
}

template<typename ItemType>
DynamicArray<ItemType>::~DynamicArray()
{
    delete[] arrayData_;
}

template<typename ItemType>
DynamicArray<ItemType>& DynamicArray<ItemType>::operator=(const DynamicArray& other)
{
    if (this != &other)
    {
        DynamicArray copy(other);
        swap(copy);
    }
    return *this;
}

template<typename ItemType>
DynamicArray<ItemType>& DynamicArray<ItemType>::operator=(DynamicArray&& other) noexcept
{
    if (this != &other)
    {
        delete[] arrayData_;

        arrayData_ = other.arrayData_;
        arrayLength_ = other.arrayLength_;
        arrayCapacity_ = other.arrayCapacity_;

        other.arrayData_ = nullptr;
        other.arrayLength_ = 0;
        other.arrayCapacity_ = 0;
    }
    return *this;
}

template<typename ItemType>
void DynamicArray<ItemType>::reserve(int newCapacity)
{
    if (newCapacity <= arrayCapacity_)
        return;

    ItemType* newArrayData = new ItemType[newCapacity];
    for (int index = 0; index < arrayLength_; index++)
        newArrayData[index] = arrayData_[index];

    delete[] arrayData_;
    arrayData_ = newArrayData;
    arrayCapacity_ = newCapacity;
}

template<typename ItemType>
void DynamicArray<ItemType>::shrink()
{
    if (arrayCapacity_ <= 4 || arrayLength_ > arrayCapacity_ / 4)
        return;

    int newCapacity = arrayCapacity_ / 2;
    if (newCapacity < 4)
        newCapacity = 4;
    if (newCapacity < arrayLength_)
        newCapacity = arrayLength_;

    ItemType* newArrayData = new ItemType[newCapacity];
    for (int index = 0; index < arrayLength_; index++)
        newArrayData[index] = arrayData_[index];

    delete[] arrayData_;
    arrayData_ = newArrayData;
    arrayCapacity_ = newCapacity;
}

template<typename ItemType>
DynamicArray<ItemType>& DynamicArray<ItemType>::operator+=(const ItemType& item)
{
    if (arrayLength_ == arrayCapacity_)
        reserve(arrayCapacity_ == 0 ? 4 : arrayCapacity_ * 2);

    arrayData_[arrayLength_] = item;
    arrayLength_++;
    return *this;
}

template<typename ItemType>
DynamicArray<ItemType>& DynamicArray<ItemType>::operator+=(const DynamicArray& other)
{
    if (this == &other)
    {
        DynamicArray copy(other);
        return *this += copy;
    }

    int oldLength = arrayLength_;
    int newLength = arrayLength_ + other.arrayLength_;
    if (newLength > arrayCapacity_)
    {
        int newCapacity = arrayCapacity_ == 0 ? 4 : arrayCapacity_;
        while (newCapacity < newLength)
            newCapacity *= 2;
        reserve(newCapacity);
    }

    for (int index = 0; index < other.arrayLength_; index++)
        arrayData_[oldLength + index] = other.arrayData_[index];

    arrayLength_ = newLength;
    return *this;
}

template<typename ItemType>
DynamicArray<ItemType> DynamicArray<ItemType>::operator+(const ItemType& item) const
{
    DynamicArray result(*this);
    result += item;
    return result;
}

template<typename ItemType>
DynamicArray<ItemType> DynamicArray<ItemType>::operator+(const DynamicArray& other) const
{
    DynamicArray result(*this);
    result += other;
    return result;
}

template<typename ItemType>
bool DynamicArray<ItemType>::operator==(const DynamicArray& other) const
{
    if (arrayLength_ != other.arrayLength_)
        return false;

    for (int index = 0; index < arrayLength_; index++)
        if (!(arrayData_[index] == other.arrayData_[index]))
            return false;

    return true;
}

template<typename ItemType>
bool DynamicArray<ItemType>::operator!=(const DynamicArray& other) const
{
    return !(*this == other);
}

template<typename ItemType>
int DynamicArray<ItemType>::getLength() const
{
    return arrayLength_;
}

template<typename ItemType>
int DynamicArray<ItemType>::getCapacity() const
{
    return arrayCapacity_;
}

template<typename ItemType>
ItemType& DynamicArray<ItemType>::operator[](const int index)
{
    assert(index >= 0 && index < arrayLength_ && "Index is out of range.");
    return arrayData_[index];
}

template<typename ItemType>
const ItemType& DynamicArray<ItemType>::operator[](const int index) const
{
    assert(index >= 0 && index < arrayLength_ && "Index is out of range.");
    return arrayData_[index];
}

template<typename ItemType>
void DynamicArray<ItemType>::swap(DynamicArray& other)
{
    std::swap(arrayData_, other.arrayData_);
    std::swap(arrayLength_, other.arrayLength_);
    std::swap(arrayCapacity_, other.arrayCapacity_);
}

template<typename ItemType>
int DynamicArray<ItemType>::find(const ItemType& item) const
{
    for (int index = 0; index < arrayLength_; index++)
        if (arrayData_[index] == item)
            return index;
    return -1;
}

template<typename ItemType>
void DynamicArray<ItemType>::sort()
{
    for (int i = 0; i < arrayLength_; i++)
        for (int j = 0; j + 1 < arrayLength_ - i; j++)
            if (arrayData_[j] > arrayData_[j + 1])
                std::swap(arrayData_[j], arrayData_[j + 1]);
}

template<typename ItemType>
bool DynamicArray<ItemType>::insert(const int index, const ItemType& item)
{
    if (index < 0 || index > arrayLength_)
        return false;

    if (arrayLength_ == arrayCapacity_)
        reserve(arrayCapacity_ == 0 ? 4 : arrayCapacity_ * 2);

    for (int i = arrayLength_; i > index; i--)
        arrayData_[i] = arrayData_[i - 1];

    arrayData_[index] = item;
    arrayLength_++;
    return true;
}

template<typename ItemType>
bool DynamicArray<ItemType>::removeAt(const int index)
{
    if (index < 0 || index >= arrayLength_)
        return false;

    for (int i = index; i < arrayLength_ - 1; i++)
        arrayData_[i] = arrayData_[i + 1];

    arrayLength_--;
    shrink();
    return true;
}

template<typename ItemType>
bool DynamicArray<ItemType>::remove(const ItemType& item)
{
    int index = find(item);
    if (index == -1)
        return false;
    return removeAt(index);
}

template<typename ItemType>
int DynamicArray<ItemType>::removeAll(const ItemType& item)
{
    int removed = 0;
    int index = 0;

    while (index < arrayLength_)
    {
        if (arrayData_[index] == item)
        {
            removeAt(index);
            removed++;
        }
        else
            index++;
    }
    return removed;
}

template<typename ItemType>
ItemType DynamicArray<ItemType>::getMin() const
{
    assert(arrayLength_ > 0 && "Array is empty.");
    ItemType result = arrayData_[0];
    for (int i = 1; i < arrayLength_; i++)
        if (arrayData_[i] < result)
            result = arrayData_[i];
    return result;
}

template<typename ItemType>
ItemType DynamicArray<ItemType>::getMax() const
{
    assert(arrayLength_ > 0 && "Array is empty.");
    ItemType result = arrayData_[0];
    for (int i = 1; i < arrayLength_; i++)
        if (arrayData_[i] > result)
            result = arrayData_[i];
    return result;
}

template<typename ItemType>
typename DynamicArray<ItemType>::iterator DynamicArray<ItemType>::insert(iterator position, const ItemType& item)
{
    if (position.arrayData_ != arrayData_ || position.elementIndex_ < 0 ||
        position.elementIndex_ > arrayLength_)
        return end();

    int index = position.elementIndex_;
    insert(index, item);
    return iterator(arrayData_, index);
}

template<typename ItemType>
typename DynamicArray<ItemType>::iterator DynamicArray<ItemType>::erase(iterator position)
{
    if (position.arrayData_ != arrayData_ || position.elementIndex_ < 0 ||
        position.elementIndex_ >= arrayLength_)
        return end();

    int index = position.elementIndex_;
    removeAt(index);
    return iterator(arrayData_, index);
}

template<typename ItemType>
typename DynamicArray<ItemType>::iterator DynamicArray<ItemType>::erase(iterator first, iterator last)
{
    if (first.arrayData_ != arrayData_ || last.arrayData_ != arrayData_ ||
        first.elementIndex_ < 0 || last.elementIndex_ < first.elementIndex_ ||
        last.elementIndex_ > arrayLength_)
        return end();

    int count = last.elementIndex_ - first.elementIndex_;
    for (int i = 0; i < count; i++)
        removeAt(first.elementIndex_);
    return iterator(arrayData_, first.elementIndex_);
}

template<typename ItemType>
typename DynamicArray<ItemType>::iterator DynamicArray<ItemType>::begin()
{
    return iterator(arrayData_, 0);
}

template<typename ItemType>
typename DynamicArray<ItemType>::iterator DynamicArray<ItemType>::end()
{
    return iterator(arrayData_, arrayLength_);
}

template<typename ItemType>
typename DynamicArray<ItemType>::const_iterator DynamicArray<ItemType>::begin() const
{
    return const_iterator(arrayData_, 0);
}

template<typename ItemType>
typename DynamicArray<ItemType>::const_iterator DynamicArray<ItemType>::end() const
{
    return const_iterator(arrayData_, arrayLength_);
}

template<typename ItemType>
typename DynamicArray<ItemType>::const_iterator DynamicArray<ItemType>::cbegin() const
{
    return const_iterator(arrayData_, 0);
}

template<typename ItemType>
typename DynamicArray<ItemType>::const_iterator DynamicArray<ItemType>::cend() const
{
    return const_iterator(arrayData_, arrayLength_);
}

template<typename ItemType>
DynamicArray<ItemType>::iterator::iterator(ItemType* arrayData, int elementIndex)
    : arrayData_(arrayData), elementIndex_(elementIndex) {
}

template<typename ItemType>
bool DynamicArray<ItemType>::iterator::operator!=(const iterator& otherIterator) const
{
    return arrayData_ != otherIterator.arrayData_ || elementIndex_ != otherIterator.elementIndex_;
}

template<typename ItemType>
bool DynamicArray<ItemType>::iterator::operator==(const iterator& otherIterator) const
{
    return !(*this != otherIterator);
}

template<typename ItemType>
typename DynamicArray<ItemType>::iterator& DynamicArray<ItemType>::iterator::operator++()
{
    ++elementIndex_;
    return *this;
}

template<typename ItemType>
typename DynamicArray<ItemType>::iterator DynamicArray<ItemType>::iterator::operator++(int)
{
    iterator copy(*this);
    ++elementIndex_;
    return copy;
}

template<typename ItemType>
ItemType& DynamicArray<ItemType>::iterator::operator*() const
{
    return arrayData_[elementIndex_];
}

template<typename ItemType>
DynamicArray<ItemType>::const_iterator::const_iterator(const ItemType* arrayData, int elementIndex)
    : arrayData_(arrayData), elementIndex_(elementIndex) {
}

template<typename ItemType>
DynamicArray<ItemType>::const_iterator::const_iterator(const iterator& other)
    : arrayData_(other.arrayData_), elementIndex_(other.elementIndex_) {
}

template<typename ItemType>
bool DynamicArray<ItemType>::const_iterator::operator!=(const const_iterator& otherIterator) const
{
    return arrayData_ != otherIterator.arrayData_ || elementIndex_ != otherIterator.elementIndex_;
}

template<typename ItemType>
bool DynamicArray<ItemType>::const_iterator::operator==(const const_iterator& otherIterator) const
{
    return !(*this != otherIterator);
}

template<typename ItemType>
typename DynamicArray<ItemType>::const_iterator& DynamicArray<ItemType>::const_iterator::operator++()
{
    ++elementIndex_;
    return *this;
}

template<typename ItemType>
typename DynamicArray<ItemType>::const_iterator DynamicArray<ItemType>::const_iterator::operator++(int)
{
    const_iterator copy(*this);
    ++elementIndex_;
    return copy;
}

template<typename ItemType>
const ItemType& DynamicArray<ItemType>::const_iterator::operator*() const
{
    return arrayData_[elementIndex_];
}

template<typename ItemType>
std::ostream& operator<<(std::ostream& outputStream, const DynamicArray<ItemType>& arrayRef)
{
    outputStream << "[";
    for (int index = 0; index < arrayRef.getLength(); index++)
    {
        outputStream << arrayRef[index];
        if (index + 1 < arrayRef.getLength())
            outputStream << ", ";
    }
    outputStream << "]";
    return outputStream;
}

template<typename ItemType>
std::istream& operator>>(std::istream& inputStream, DynamicArray<ItemType>& arrayRef)
{
    ItemType value;
    if (inputStream >> value)
        arrayRef += value;
    return inputStream;
}
