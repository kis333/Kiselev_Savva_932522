#include "BooleanMatrix.h"

#include <cassert>
#include <cstring>
#include <stdexcept>
#include <utility>

BooleanMatrix::BooleanMatrix(const uint32_t numRows, const uint32_t numCols, const bool initialValue)
{
    for (uint32_t rowIndex = 0; rowIndex < numRows; rowIndex++)
    {
        matrixData_ += BooleanVector(numCols, initialValue);
    }
}

BooleanMatrix::BooleanMatrix(const char* const* data, const uint32_t numRows, const uint32_t numCols)
{
    if (numRows > 0 && data == nullptr)
        throw std::invalid_argument("data is null");

    for (uint32_t rowIndex = 0; rowIndex < numRows; rowIndex++)
    {
        if (data[rowIndex] == nullptr)
            throw std::invalid_argument("row is null");
        if (static_cast<uint32_t>(std::strlen(data[rowIndex])) < numCols)
            throw std::invalid_argument("row too short");

        BooleanVector row(numCols, false);
        for (uint32_t colIndex = 0; colIndex < numCols; colIndex++)
            row[colIndex] = (data[rowIndex][colIndex] == '1');

        matrixData_ += row;
    }
}


uint32_t BooleanMatrix::getRowsCount() const
{
    return static_cast<uint32_t>(matrixData_.getLength());
}

uint32_t BooleanMatrix::getColumnsCount() const
{
    if (matrixData_.getLength() == 0) return 0;

    return matrixData_[0].getBitsCount();
}


void BooleanMatrix::swap(BooleanMatrix& other) noexcept
{
    matrixData_.swap(other.matrixData_);
}


void BooleanMatrix::outputToConsole() const
{
    uint32_t numRows = getRowsCount();
    for (uint32_t rowIndex = 0; rowIndex < numRows; rowIndex++)
    {
        std::cout << matrixData_[rowIndex];   
        std::cout << std::endl;
    }
}

std::ostream& operator<<(std::ostream& os, const BooleanMatrix& m)
{
    for (uint32_t rowIndex = 0; rowIndex < m.getRowsCount(); rowIndex++)
    {
        for (uint32_t colIndex = 0; colIndex < m.getColumnsCount(); colIndex++)
            os << (m.matrixData_[rowIndex][colIndex] ? '1' : '0');
        os << '\n';
    }
    return os;
}

std::istream& operator>>(std::istream& is, BooleanMatrix& m)
{
    uint32_t numRows = 0, numCols = 0;
    if (!(is >> numRows >> numCols))
        return is;

    BooleanMatrix tmp(numRows, numCols, false);
    for (uint32_t rowIndex = 0; rowIndex < numRows; rowIndex++)
    {
        for (uint32_t colIndex = 0; colIndex < numCols; colIndex++)
        {
            char ch = 0;
            if (!(is >> ch))
                return is;
            if (ch != '0' && ch != '1')
            {
                is.setstate(std::ios::failbit);
                return is;
            }
            tmp.matrixData_[rowIndex][colIndex] = (ch == '1');
        }
    }
    m = std::move(tmp);
    return is;
}

uint32_t BooleanMatrix::getWeight() const
{
    uint32_t weight = 0;
    for (uint32_t rowIndex = 0; rowIndex < getRowsCount(); rowIndex++)
        weight += matrixData_[rowIndex].getWeight();
    return weight;
}

uint32_t BooleanMatrix::getRowWeight(const uint32_t rowIndex) const
{
    assert(rowIndex < getRowsCount() && "Row index out of range");
    return matrixData_[rowIndex].getWeight();
}


BooleanVector BooleanMatrix::getAllRowsConjunction() const
{
    if (getRowsCount() == 0)
        return BooleanVector();

    BooleanVector result = matrixData_[0];
    for (uint32_t rowIndex = 1; rowIndex < getRowsCount(); rowIndex++)
    {
        for (uint32_t colIndex = 0; colIndex < getColumnsCount(); colIndex++)
            result[colIndex] = result[colIndex] && matrixData_[rowIndex][colIndex];
    }
    return result;
}

BooleanVector BooleanMatrix::getAllRowsDisjunction() const
{
    if (getRowsCount() == 0) return BooleanVector();

    BooleanVector result = matrixData_[0];
    for (uint32_t rowIndex = 1; rowIndex < getRowsCount(); rowIndex++)
        result |= matrixData_[rowIndex];
    return result;
}

void BooleanMatrix::invertComponent(const uint32_t rowIndex, const uint32_t colIndex)
{
    assert(rowIndex < getRowsCount() && "Row index out of range");
    assert(colIndex < getColumnsCount() && "Column index out of range");
    matrixData_[rowIndex][colIndex] = !matrixData_[rowIndex][colIndex];
}

void BooleanMatrix::invertComponents(const uint32_t rowIndex, const uint32_t colIndex, const uint32_t count)
{
    assert(rowIndex < getRowsCount() && "Row index out of range");
    assert(colIndex + count <= getColumnsCount() && "Range out of bounds");
    for (uint32_t offset = 0; offset < count; offset++)
        matrixData_[rowIndex][colIndex + offset] = !matrixData_[rowIndex][colIndex + offset];
}

void BooleanMatrix::setComponent(const uint32_t rowIndex, const uint32_t colIndex, const bool value)
{
    assert(rowIndex < getRowsCount() && "Row index out of range");
    assert(colIndex < getColumnsCount() && "Column index out of range");
    matrixData_[rowIndex][colIndex] = value;
}

void BooleanMatrix::setComponents(const uint32_t rowIndex, const uint32_t colIndex,
    const uint32_t count, const bool value)
{
    assert(rowIndex < getRowsCount() && "Row index out of range");
    assert(colIndex + count <= getColumnsCount() && "Range out of bounds");
    for (uint32_t offset = 0; offset < count; offset++)
        matrixData_[rowIndex][colIndex + offset] = value;
}


BooleanVector& BooleanMatrix::operator[](const uint32_t rowIndex)
{
    return matrixData_[rowIndex];
}

const BooleanVector& BooleanMatrix::operator[](const uint32_t rowIndex) const
{
    return matrixData_[rowIndex];
}

BooleanMatrix BooleanMatrix::operator&(const BooleanMatrix& other) const
{
    BooleanMatrix result(*this);
    result &= other;
    return result;
}

BooleanMatrix& BooleanMatrix::operator&=(const BooleanMatrix& other)
{
    assert(getRowsCount() == other.getRowsCount() && "Row count mismatch");
    assert(getColumnsCount() == other.getColumnsCount() && "Column count mismatch");
    for (uint32_t rowIndex = 0; rowIndex < getRowsCount(); rowIndex++)
    {
        for (uint32_t colIndex = 0; colIndex < getColumnsCount(); colIndex++)
            matrixData_[rowIndex][colIndex] =
            matrixData_[rowIndex][colIndex] && other.matrixData_[rowIndex][colIndex];
    }
    return *this;
}

BooleanMatrix BooleanMatrix::operator|(const BooleanMatrix& other) const
{
    BooleanMatrix result(*this);
    result |= other;
    return result;
}

BooleanMatrix& BooleanMatrix::operator|=(const BooleanMatrix& other)
{
    assert(getRowsCount() == other.getRowsCount() && "Row count mismatch");
    assert(getColumnsCount() == other.getColumnsCount() && "Column count mismatch");
    for (uint32_t rowIndex = 0; rowIndex < getRowsCount(); rowIndex++)
        matrixData_[rowIndex] |= other.matrixData_[rowIndex];
    return *this;
}

BooleanMatrix BooleanMatrix::operator^(const BooleanMatrix& other) const
{
    BooleanMatrix result(*this);
    result ^= other;
    return result;
}

BooleanMatrix& BooleanMatrix::operator^=(const BooleanMatrix& other)
{
    assert(getRowsCount() == other.getRowsCount() && "Row count mismatch");
    assert(getColumnsCount() == other.getColumnsCount() && "Column count mismatch");
    for (uint32_t rowIndex = 0; rowIndex < getRowsCount(); rowIndex++)
    {
        for (uint32_t colIndex = 0; colIndex < getColumnsCount(); colIndex++)
            matrixData_[rowIndex][colIndex] =
            matrixData_[rowIndex][colIndex] != other.matrixData_[rowIndex][colIndex];
    }
    return *this;
}

BooleanMatrix BooleanMatrix::operator~() const
{
    BooleanMatrix result(getRowsCount(), getColumnsCount(), false);
    for (uint32_t rowIndex = 0; rowIndex < getRowsCount(); rowIndex++)
    {
        for (uint32_t colIndex = 0; colIndex < getColumnsCount(); colIndex++)
            result.matrixData_[rowIndex][colIndex] = !matrixData_[rowIndex][colIndex];
    }
    return result;
}
