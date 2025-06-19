#pragma once
#include <vector>
#include <unordered_map>
#include "types.h"

#define BYTE_LEN 8
#define MAX_TOKEN_LEN 8
#define CHUNK_SIZE 256

uint32_t toLittleEndian(uint32_t value);
double calcEntropy(const std::unordered_map<char, Symbol>& map);
double calcEfficiency(const std::unordered_map<char, Symbol>& map);