#include "general.h"
#include <vector>

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