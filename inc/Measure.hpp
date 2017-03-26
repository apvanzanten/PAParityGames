#ifndef MEASURE_HPP
#define MEASURE_HPP

#include <string>
#include <vector>
#include <iostream>

namespace PAPG {

class Measure
{
private:
	std::vector<unsigned> oddValues;
	size_t size;
	const Measure * maxValue;
	bool isToppedOut;

	inline static size_t convertIndex(size_t index) {
		if(index == 0) return 0;
		return (index - 1) / 2;
	}
public:
	Measure(const Measure & maxValue);
	
	explicit Measure(std::vector<unsigned> maxValues); // special constructor for creation of max measure
	
	bool setValue(size_t index, unsigned newValue);
	unsigned getValue(size_t index) const;

	inline bool isTop() const { return isToppedOut; }
	inline void makeTop() { isToppedOut = true; }

	Measure & operator=(const Measure & rhs);

	bool operator==(const Measure & rhs) const;
	bool operator!=(const Measure & rhs) const;

	bool operator<(const Measure & rhs) const;
	bool operator>(const Measure & rhs) const;
	bool operator<=(const Measure & rhs) const;
	bool operator>=(const Measure & rhs) const;

	bool partialGreater(size_t boundary, const Measure & other) const;
	bool partialGreaterOrEqual(size_t boundary, const Measure & other) const;

	bool partialIncrementIfAble(size_t boundary);

	void makePartialEqualOf(size_t boundary, const Measure & other);

	inline size_t getSize() const { return size; }

	friend std::ostream & operator<<(std::ostream & stream, const Measure & measure);

};

} // PAPC

#endif // MEASURE_HPP
