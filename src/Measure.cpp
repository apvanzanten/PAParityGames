#include "Measure.hpp"
#include <algorithm>
#include <stdexcept>

namespace PAPG {

Measure::Measure(const Measure& maxValue)
    : oddValues(((maxValue.getSize() + 1) / 2), 0)
    , size(maxValue.getSize())
    , maxValue(&maxValue)
    , isToppedOut(false)
{
}

Measure::Measure(std::vector<unsigned> maxValues)
    : oddValues(((maxValues.size() + 1) / 2), 0)
    , size(maxValues.size())
    , maxValue(this)
    , isToppedOut(false)

{
    for (size_t i = 0; i < oddValues.size(); i++) {
        oddValues[i] = maxValues[((i * 2) + 1)];
    }
}

bool Measure::setValue(size_t index, unsigned newValue)
{
    if (index >= size)
        throw std::out_of_range("setValue: Measure index out of range!");

    if (index % 2 == 0 || newValue > maxValue->getValue(index)) {
        return false;
    }

    oddValues[convertIndex(index)] = newValue;
    return true;
}

unsigned Measure::getValue(size_t index) const
{
    if (index >= size)
        throw std::out_of_range("getValue(): Measure index out of range!");

    if (index % 2 == 0)
        return 0;

    return oddValues[convertIndex(index)];
}

Measure& Measure::operator=(const Measure& rhs)
{
    this->oddValues = rhs.oddValues;
    this->size = rhs.size;
    this->maxValue = rhs.maxValue;
    this->isToppedOut = rhs.isToppedOut;

    return *this;
}

bool Measure::operator==(const Measure& rhs) const
{
    return (this == &rhs)
        || (this->isTop() && rhs.isTop()) // if both are top, they are considered equal
        || (!this->isTop() && !rhs.isTop() // if not both of them are top, neither should be
               && this->oddValues == rhs.oddValues
               && this->size == rhs.size);
}

bool Measure::operator!=(const Measure& rhs) const
{
    return !(*this == rhs);
}

bool Measure::operator<(const Measure& rhs) const
{
    if (rhs.isTop()) {
        return true;
    } else if (this->isTop()) {
        return false;
    }

    return std::lexicographical_compare(this->oddValues.begin(), this->oddValues.end(), rhs.oddValues.begin(), rhs.oddValues.end());
}

bool Measure::operator>(const Measure& rhs) const
{
    if (this->isTop()) {
        return true;
    } else if (rhs.isTop()) {
        return false;
    }

    return rhs < *this;
}

bool Measure::operator<=(const Measure& rhs) const
{
    if (rhs.isTop()) {
        return true;
    } else if (this->isTop()) {
        return false;
    }

    return !(*this > rhs);
}

bool Measure::operator>=(const Measure& rhs) const
{
    if (this->isTop()) {
        return true;
    } else if (rhs.isTop()) {
        return false;
    }

    return !(*this < rhs);
}

bool Measure::partialGreater(size_t boundary, const Measure& rhs) const
{
    if (boundary >= size || boundary >= rhs.size)
        throw std::out_of_range("partialGreater(): Measure boundary out of range!");

    if (isTop()) {
        return true;
    } else if (rhs.isTop()) {
        return false;
    }

    for (size_t i = 0; i < convertIndex(boundary) + 1; i++) {
        if (oddValues[i] > rhs.oddValues[i]) {
            return true;
        } else if (oddValues[i] < rhs.oddValues[i]) {
            return false;
        } // leaves case where they are equal
    }
    return false;
}

bool Measure::partialGreaterOrEqual(size_t boundary, const Measure& rhs) const
{
    return !(rhs.partialGreater(boundary, *this));
}

bool Measure::partialIncrementIfAble(size_t boundary)
{
    for (int i = convertIndex(boundary); i >= 0; i--) {
        if (oddValues[i] < maxValue->oddValues[i]) {
            // value lower than max, we can incremement it
            oddValues[i]++;
            return true;
        }
    }

    return false;
}

void Measure::makePartialEqualOf(size_t boundary, const Measure& other)
{
    if (boundary >= size || boundary >= other.size) {
        throw std::out_of_range("makePartialEqualOf(): Measure boundary out of range!");
    }

    if (this == &other) {
        // if this and other are equal it means we are trying to make ourselves equal to ourselves, do nothing and return.
        return;
    }

    if (other.isTop()) {
        makeTop();
        return;
    }

    size_t i = 0;
    for (; i < (boundary + 1) / 2; i++) {
        oddValues[i] = other.oddValues[i];
    }
    for (; i < oddValues.size(); i++) {
        oddValues[i] = 0;
    }
}

std::ostream& operator<<(std::ostream& stream, const Measure& measure)
{
    stream << "(";

    if (measure.isTop()) {
        stream << "top";
    } else {
        for (size_t i = 0; i < measure.getSize(); i++) {
            stream << measure.getValue(i);
            if (i + 1 < measure.getSize()) {
                stream << ", ";
            }
        }
    }
    stream << ")";

    return stream;
}

} // PAPC