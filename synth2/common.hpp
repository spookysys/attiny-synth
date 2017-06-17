#pragma once
#include <cstdint>
#include <avr/pgmspace.h>

#define likely(x)      __builtin_expect(!!(x), 1)
#define unlikely(x)    __builtin_expect(!!(x), 0)
