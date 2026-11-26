#pragma once

#include <iostream>
#include <cstdint>
#include "BooleanVector.h"
#include "DynamicArray3.h"

class BooleanMatrix
{
public:
    BooleanMatrix() = default;
    BooleanMatrix(const uint32_t numRows, const uint32_t numCols, const bool initialValue = false);
    BooleanMatrix(const char* const* data, const uint32_t numRows, const uint32_t numCols);
    BooleanMatrix(const BooleanMatrix&) = default;
    BooleanMatrix(BooleanMatrix&&) noexcept = default;
    ~BooleanMatrix() = default;

    BooleanMatrix& operator=(const BooleanMatrix&) = default;
    BooleanMatrix& operator=(BooleanMatrix&&) noexcept = default;

    void outputToConsole() const;

    uint32_t getRowsCount() const;
    uint32_t getColumnsCount() const;

    void swap(BooleanMatrix& other) noexcept;

    friend std::ostream& operator<<(std::ostream& os, const BooleanMatrix& m);
    friend std::istream& operator>>(std::istream& is, BooleanMatrix& m);

    uint32_t getWeight() const;
    uint32_t getRowWeight(const uint32_t rowIndex) const;

    BooleanVector getAllRowsConjunction() const;
    BooleanVector getAllRowsDisjunction() const;

    void invertComponent(const uint32_t rowIndex, const uint32_t colIndex);
    void invertComponents(const uint32_t rowIndex, const uint32_t colIndex, const uint32_t count);
    void setComponent(const uint32_t rowIndex, const uint32_t colIndex, const bool value);
    void setComponents(const uint32_t rowIndex, const uint32_t colIndex, const uint32_t count, const bool value);

    BooleanVector& operator[](const uint32_t rowIndex);
    const BooleanVector& operator[](const uint32_t rowIndex) const;

    BooleanMatrix operator&(const BooleanMatrix& other) const;
    BooleanMatrix& operator&=(const BooleanMatrix& other);

    BooleanMatrix operator|(const BooleanMatrix& other) const;
    BooleanMatrix& operator|=(const BooleanMatrix& other);

    BooleanMatrix operator^(const BooleanMatrix& other) const;
    BooleanMatrix& operator^=(const BooleanMatrix& other);

    BooleanMatrix operator~() const;

private:
    DynamicArray<BooleanVector> matrixData_;
};
