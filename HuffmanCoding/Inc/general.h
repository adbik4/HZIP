#pragma once
#include <vector>

#define BYTE_LEN 8

uint32_t toLittleEndian(uint32_t value);
bool getBitAt(const std::vector<char>& vec, const uint32_t& idx);