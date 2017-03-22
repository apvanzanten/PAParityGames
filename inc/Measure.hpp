#ifndef MEASURE_HPP
#define MEASURE_HPP

#include <string>
#include <vector>

namespace PAPC {

class Measure
{
private:
	std::vector<unsigned> oddValues;
	size_t size;
	const Measure * maxValue;


	inline static size_t convertIndex(size_t index) {
		if(index == 0) return 0;
		return (index - 1) / 2;
	}
public:
	Measure(const Measure & maxValue);
	
	Measure(std::vector<unsigned> maxValues); // special constructor for creation of max measure
	
	bool setValue(size_t index, unsigned newValue);
	unsigned getValue(size_t index) const;

	bool isTop() const;

	Measure & operator=(const Measure & rhs);

	bool operator==(const Measure & rhs) const;
	bool operator!=(const Measure & rhs) const;


	bool partialGreater(size_t boundary, const Measure & other) const;
	bool partialGreaterOrEqual(size_t boundary, const Measure & other) const;

	bool makePartialSuccessorOf(size_t boundary, const Measure & other);

	inline size_t getSize() const { return size; }

};

} // PAPC

#endif // MEASURE_HPP
