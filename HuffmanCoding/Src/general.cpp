#include "general.h"
#include <vector>
#include <unordered_map>
#include "types.h"

uint32_t toLittleEndian(uint32_t value) {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	return value;  // Already little endian
#else
	return ((value & 0x000000FF) << 24) |
		((value & 0x0000FF00) << 8) |
		((value & 0x00FF0000) >> 8) |
		((value & 0xFF000000) >> 24);
#endif
}

double calcEntropy(const std::unordered_map<char, Symbol>& map) {
	double entropy = 0;
	for (const auto& [key, value] : map) {
		entropy += -value.freq * log2(value.freq);
	}
	return entropy;
}

double calcEfficiency(const std::unordered_map<char, Symbol>& map) {
	double entropy = 0;
	double expectedEncodingLen = 0;
	for (const auto& [key, value] : map) {
		entropy += -value.freq * log2(value.freq);
		expectedEncodingLen += value.freq * value.encoding.length;
	}
	return entropy / expectedEncodingLen;
}