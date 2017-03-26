#include "Measure.hpp"
#include <algorithm>
#include <stdexcept>

namespace PAPG {

Measure::Measure(const Measure& maxValue)
    : oddValues(((maxValue.getSize() + 1) / 2), 0)
    , size(maxValue.getSize())
    , maxValue(&maxValue)
{
}

Measure::Measure(std::vector<unsigned> maxValues)
    : oddValues(((maxValues.size() + 1) / 2), 0)
    , size(maxValues.size())
    , maxValue(this)
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

bool Measure::isTop() const
{
    return *this == *maxValue;
}

void Measure::makeTop() 
{   
    for(size_t i = 0; i < oddValues.size(); i++){
        oddValues[i] = maxValue->oddValues[i];
    }
}

Measure& Measure::operator=(const Measure& rhs)
{
    this->oddValues = rhs.oddValues;
    this->size = rhs.size;
    this->maxValue = rhs.maxValue;

    return *this;
}

bool Measure::operator==(const Measure& rhs) const
{
    return (this == &rhs)
        || (this->oddValues == rhs.oddValues
               && this->size == rhs.size);
}

bool Measure::operator!=(const Measure& rhs) const
{
    return !(*this == rhs);
}

bool Measure::operator<(const Measure & rhs) const {
    return std::lexicographical_compare(this->oddValues.begin(), this->oddValues.end(), rhs.oddValues.begin(), rhs.oddValues.end());
}

bool Measure::operator>(const Measure & rhs) const {
    return rhs < *this;    
}

bool Measure::operator<=(const Measure & rhs) const {
    return !(rhs < *this);
}

bool Measure::operator>=(const Measure & rhs) const {
    return !(*this < rhs);
}


bool Measure::partialGreater(size_t boundary, const Measure& rhs) const
{
    if (boundary >= size || boundary >= rhs.size)
        throw std::out_of_range("partialGreater(): Measure boundary out of range!");

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

bool Measure::makePartialSuccessorOf(size_t boundary, const Measure& other)
{
    if (boundary >= size || boundary >= other.size){
        throw std::out_of_range("makePartialSuccessorOf(): Measure boundary out of range!");
    }

    if(other.isTop()){
        makeTop();
        return false;
    }

    this->oddValues = other.oddValues;

    for (int i = convertIndex(boundary); i >= 0; i--) {
        if (oddValues[i] < maxValue->oddValues[i]) {
            // value lower, we an incremement it
            oddValues[i]++;
            return true;
        }
    }

    // if we reach this point, that means we can't go any higher
    makeTop();

    return false;
}

void Measure::makePartialEqualOf(size_t boundary, const PAPG::Measure& other)
{
    if (boundary >= size || boundary >= other.size){
        throw std::out_of_range("makePartialEqualOf(): Measure boundary out of range!");
    }

    if(other.isTop()){
        makeTop();
        return;
    }

    size_t i = 0;
    for (; i <= convertIndex(boundary); i++){
        oddValues[i] = other.oddValues[i];
    }
    for(; i < oddValues.size(); i++){
        oddValues[i] = 0;
    }
}

} // PAPC