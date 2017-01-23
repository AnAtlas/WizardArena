#pragma once
#include <stdint.h>

using Bitset = uint32_t;

class Bitmask {
private:
	Bitset bits;
	
public:
	Bitmask() : bits(0) {}
	Bitmask(const Bitset& bits) : bits(bits) {}

	Bitset getMask() const { return bits; }
	void setMask(const Bitset& value) { bits = value; }

	bool matches(const Bitmask& bits, const Bitset& relevant = 0) const {
		return (relevant ? ((bits.getMask() & relevant) == (this->bits & relevant)) : (bits.getMask() == this->bits));
	}

	bool getBit(const unsigned int& pos) const{
		return ((bits&(1 << pos)) != 0);
	}

	void turnOnBit(const unsigned int& pos) {
		bits |= 1 << pos;
	}

	void turnOnBits(const Bitset& onBits) {
		bits |= onBits;
	}

	void clearBit(const unsigned int& pos) {
		bits &= ~(1 << pos);
	}

	void toggleBit(const unsigned int& pos) {
		bits ^= 1 << pos;
	}

	void clear() { bits = 0; }
};