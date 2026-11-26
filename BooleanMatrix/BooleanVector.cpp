#include "BooleanVector.h"
#include <cstring>
#include <stdexcept>
#include <algorithm>


BooleanVector::BooleanVector() = default;

BooleanVector::BooleanVector(const uint32_t numBits, const bool initialValue)
    : numBits_(numBits)
{
    if (numBits == 0) {
        numBytes_ = 0;
        vectorData_ = nullptr;
        return;
    }
    numBytes_ = (numBits + 7) / 8;
    vectorData_ = new uint8_t[numBytes_];
    std::memset(vectorData_, initialValue ? 0xFF : 0x00, numBytes_);
}

BooleanVector::BooleanVector(const char* bitString)
{
    if (!bitString) {
        numBits_ = 0;
        numBytes_ = 0;
        vectorData_ = nullptr;
        return;
    }
    numBits_ = static_cast<uint32_t>(std::strlen(bitString));
    if (numBits_ == 0) {
        numBytes_ = 0;
        vectorData_ = nullptr;
        return;
    }
    numBytes_ = (numBits_ + 7) / 8;
    vectorData_ = new uint8_t[numBytes_]();  

    for (uint32_t i = 0; i < numBits_; ++i) {
        char c = bitString[i];
        if (c == '1')
            vectorData_[i / 8] |= (1u << (i % 8));
        else if (c != '0')
            throw std::invalid_argument("Bit string must contain only '0' and '1'");
    }
}

BooleanVector::BooleanVector(const BooleanVector& other)
    : numBits_(other.numBits_), numBytes_(other.numBytes_)
{
    if (numBytes_ == 0) {
        vectorData_ = nullptr;
        return;
    }
    vectorData_ = new uint8_t[numBytes_];
    std::memcpy(vectorData_, other.vectorData_, numBytes_);
}

BooleanVector::~BooleanVector()
{
    delete[] vectorData_;
}

BooleanVector BooleanVector::otr(uint32_t start, uint32_t count) const
{
    if (count > numBits_)
        count = numBits_;

    BooleanVector r(count);
    uint32_t sb = start / 8, sh = start % 8;
    

    for (uint32_t k = 0; k < r.numBytes_; ++k) {
        r.vectorData_[k] = vectorData_[sb + k] >> sh;
        if (sh && sb + k + 1 < numBytes_)
            r.vectorData_[k] |= vectorData_[sb + k + 1] << (8 - sh);
    }
    return r;
}

BooleanVector& BooleanVector::operator=(const BooleanVector& other)
{
    if (this == &other) return *this;
    BooleanVector tmp(other);
    swap(tmp);
    return *this;
}


void BooleanVector::checkIndex(const uint32_t bitIndex) const
{
    if (bitIndex >= numBits_)
        throw std::out_of_range("Bit index out of range");
}

uint32_t BooleanVector::getBitsCount() const { return numBits_; }

uint32_t BooleanVector::getWeight() const
{
    uint32_t count = 0;
    for (uint32_t i = 0; i < numBytes_; ++i) {
        uint8_t b = vectorData_[i];
        while (b) { count += (b & 1u); b >>= 1; }
    }
    return count;
}

void BooleanVector::swap(BooleanVector& other)
{
    std::swap(vectorData_, other.vectorData_);
    std::swap(numBits_, other.numBits_);
    std::swap(numBytes_, other.numBytes_);
}


bool BooleanVector::operator[](const uint32_t bitIndex) const
{
    checkIndex(bitIndex);
    return (vectorData_[bitIndex / 8] >> (bitIndex % 8)) & 1u;
}

BooleanVector::BitReference BooleanVector::operator[](const uint32_t bitIndex)
{
    checkIndex(bitIndex);
    return BitReference(&vectorData_[bitIndex / 8], bitIndex % 8);
}

void BooleanVector::setValue(const uint32_t bitIndex, const bool newValue)
{
    checkIndex(bitIndex);
    uint8_t mask = static_cast<uint8_t>(1u << (bitIndex % 8));
    if (newValue)
        vectorData_[bitIndex / 8] |= mask;
    else
        vectorData_[bitIndex / 8] &= static_cast<uint8_t>(~mask);
}

void BooleanVector::setRange(const uint32_t startIndex, const uint32_t count, const bool newValue)
{
    for (uint32_t i = 0; i < count; ++i)
        setValue(startIndex + i, newValue);
}

void BooleanVector::setAll(const bool newValue)
{
    if (numBytes_ == 0) return;
    std::memset(vectorData_, newValue ? 0xFF : 0x00, numBytes_);
}


void BooleanVector::invert(const uint32_t bitIndex)
{
    checkIndex(bitIndex);
    vectorData_[bitIndex / 8] ^= static_cast<uint8_t>(1u << (bitIndex % 8));
}

void BooleanVector::invertRange(const uint32_t startIndex, const uint32_t count)
{
    for (uint32_t i = 0; i < count; ++i)
        invert(startIndex + i);
}

void BooleanVector::invertAll()
{
    for (uint32_t i = 0; i < numBytes_; ++i)
        vectorData_[i] = static_cast<uint8_t>(~vectorData_[i]);
}

BooleanVector BooleanVector::operator~() const
{
    BooleanVector result(*this);
    result.invertAll();
    return result;
}

BooleanVector& BooleanVector::operator&=(const BooleanVector& other)
{
    uint32_t n = std::min(numBytes_, other.numBytes_);
    for (uint32_t i = 0; i < n; ++i)
        vectorData_[i] &= other.vectorData_[i];
    return *this;
}

BooleanVector& BooleanVector::operator|=(const BooleanVector& other)
{
    uint32_t n = std::min(numBytes_, other.numBytes_);
    for (uint32_t i = 0; i < n; ++i)
        vectorData_[i] |= other.vectorData_[i];
    return *this;
}

BooleanVector& BooleanVector::operator^=(const BooleanVector& other)
{
    uint32_t n = std::min(numBytes_, other.numBytes_);
    for (uint32_t i = 0; i < n; ++i)
        vectorData_[i] ^= other.vectorData_[i];
    return *this;
}

BooleanVector operator&(BooleanVector lhs, const BooleanVector& rhs) { lhs &= rhs; return lhs; }
BooleanVector operator|(BooleanVector lhs, const BooleanVector& rhs) { lhs |= rhs; return lhs; }
BooleanVector operator^(BooleanVector lhs, const BooleanVector& rhs) { lhs ^= rhs; return lhs; }


BooleanVector& BooleanVector::operator<<=(uint32_t shift)
{
    if (shift >= numBits_) { setAll(false); return *this; }
    if (shift == 0) return *this;

    uint8_t* tmp = new uint8_t[numBytes_]();
    for (uint32_t i = shift; i < numBits_; ++i) {
        if ((*this)[i - shift])
            tmp[i / 8] |= static_cast<uint8_t>(1u << (i % 8));
    }
    delete[] vectorData_;
    vectorData_ = tmp;
    return *this;
}

BooleanVector& BooleanVector::operator>>=(uint32_t shift)
{
    if (shift >= numBits_) { setAll(false); return *this; }
    if (shift == 0) return *this;

    uint8_t* tmp = new uint8_t[numBytes_]();
    for (uint32_t i = 0; i + shift < numBits_; ++i) {
        if ((*this)[i + shift])
            tmp[i / 8] |= static_cast<uint8_t>(1u << (i % 8));
    }
    delete[] vectorData_;
    vectorData_ = tmp;
    return *this;
}

BooleanVector operator<<(BooleanVector v, uint32_t shift) { v <<= shift; return v; }
BooleanVector operator>>(BooleanVector v, uint32_t shift) { v >>= shift; return v; }


BooleanVector::BitReference::BitReference(uint8_t* bytePtr, uint32_t bitIndex)
    : bytePtr_(bytePtr), bitIndex_(bitIndex) {
}

bool BooleanVector::BitReference::get() const
{
    return (*bytePtr_ >> bitIndex_) & 1u;
}

void BooleanVector::BitReference::set(bool value)
{
    uint8_t mask = static_cast<uint8_t>(1u << bitIndex_);
    if (value) *bytePtr_ |= mask;
    else       *bytePtr_ &= static_cast<uint8_t>(~mask);
}

BooleanVector::BitReference::operator bool() const { return get(); }

BooleanVector::BitReference& BooleanVector::BitReference::operator=(const bool newValue)
{
    set(newValue);
    return *this;
}

BooleanVector::BitReference& BooleanVector::BitReference::operator=(const BitReference& other)
{
    set(other.get());
    return *this;
}

bool BooleanVector::BitReference::operator==(const bool value) const { return get() == value; }
bool BooleanVector::BitReference::operator!=(const bool value) const { return get() != value; }
bool BooleanVector::BitReference::operator==(const BitReference& other) const { return get() == other.get(); }
bool BooleanVector::BitReference::operator!=(const BitReference& other) const { return get() != other.get(); }

BooleanVector::BitReference& BooleanVector::BitReference::operator&=(bool value) { set(get() && value); return *this; }
BooleanVector::BitReference& BooleanVector::BitReference::operator|=(bool value) { set(get() || value); return *this; }
BooleanVector::BitReference& BooleanVector::BitReference::operator^=(bool value) { set(get() != value); return *this; }

BooleanVector::BitReference& BooleanVector::BitReference::flip() { set(!get()); return *this; }


std::istream& operator>>(std::istream& inputStream, BooleanVector& booleanVector)
{
    std::string bitString;
    if (!(inputStream >> bitString))
        return inputStream;
    booleanVector = BooleanVector(bitString.c_str());
    return inputStream;
}

std::ostream& operator<<(std::ostream& outputStream, const BooleanVector& booleanVector)
{
    uint32_t n = booleanVector.getBitsCount();
    for (uint32_t i = 0; i < n; ++i)
        outputStream << (booleanVector[i] ? '1' : '0');
    return outputStream;
}