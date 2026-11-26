#pragma once
#include <cstdint>
#include <iostream>

class BooleanVector
{
public:
    class BitReference;

    BooleanVector();
    BooleanVector(const uint32_t numBits, const bool initialValue = false);
    BooleanVector(const char* bitString);
    BooleanVector(const BooleanVector& other);
    ~BooleanVector();

    BooleanVector& operator=(const BooleanVector& other);

    uint32_t getBitsCount() const;
    uint32_t getWeight() const;

    void swap(BooleanVector& other);
 
    bool operator[](const uint32_t bitIndex) const;
    BitReference operator[](const uint32_t bitIndex);

    void setValue(const uint32_t bitIndex, const bool newValue);
    void setRange(const uint32_t startIndex, const uint32_t count, const bool newValue);
    void setAll(const bool newValue);

    void invert(const uint32_t bitIndex);
    void invertRange(const uint32_t startIndex, const uint32_t count);
    void invertAll();

    BooleanVector& operator&=(const BooleanVector& other);
    BooleanVector& operator|=(const BooleanVector& other);
    BooleanVector& operator^=(const BooleanVector& other);

    BooleanVector otr(uint32_t start, uint32_t count) const;

    BooleanVector operator~() const;
    BooleanVector& operator<<=(uint32_t shift);
    BooleanVector& operator>>=(uint32_t shift);

    friend BooleanVector operator&(BooleanVector lhs, const BooleanVector& rhs);
    friend BooleanVector operator|(BooleanVector lhs, const BooleanVector& rhs);
    friend BooleanVector operator^(BooleanVector lhs, const BooleanVector& rhs);
    friend BooleanVector operator<<(BooleanVector v, uint32_t shift);
    friend BooleanVector operator>>(BooleanVector v, uint32_t shift);

private:
    uint8_t* vectorData_ = nullptr;
    uint32_t numBits_ = 0;
    uint32_t numBytes_ = 0;

    void checkIndex(const uint32_t bitIndex) const;
};

class BooleanVector::BitReference
{
public:
    BitReference(uint8_t* bytePtr, uint32_t bitIndex);

    operator bool() const;

    BitReference& operator=(const bool newValue);
    BitReference& operator=(const BitReference& other);

    bool operator==(const bool value) const;
    bool operator!=(const bool value) const;
    bool operator==(const BitReference& other) const;
    bool operator!=(const BitReference& other) const;

    BitReference& operator&=(bool value);
    BitReference& operator|=(bool value);
    BitReference& operator^=(bool value);

    BitReference& flip();

private:
    uint8_t* bytePtr_;
    uint32_t bitIndex_;
    bool get() const;
    void set(bool value);
};

std::istream& operator>>(std::istream& inputStream, BooleanVector& booleanVector);
std::ostream& operator<<(std::ostream& outputStream, const BooleanVector& booleanVector);
