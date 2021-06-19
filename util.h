/*
 * Utility functions, some of which are intended to be inlined.
 */
#pragma once


inline uint32_t
fast_hash(uint64_t line_addr, uint64_t modulo)
{
    uint32_t res = 0;
    uint64_t tmp = line_addr;
    for (uint32_t i = 0; i < 4; ++i) {
        res ^= (uint32_t) ( ((uint64_t) 0xffff) & tmp);
        tmp = tmp >> 16;
    }
    return res % modulo;
}
