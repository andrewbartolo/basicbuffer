/*
 * Common definitions for typedefs, enums, etc.
 */
#pragma once

#include <stdint.h>


// use different signedness to get around polymorphism rules
// an address shifted by log_2(line size)
typedef int64_t line_addr_t;
// a raw address by itself
typedef uint64_t addr_t;

typedef enum {
    BUFFER_ALLOCATION_POLICY_AORW,
    BUFFER_ALLOCATION_POLICY_AOWO,
} allocation_policy_t;

typedef enum {
    BUFFER_EVICTION_POLICY_LRU,
    BUFFER_EVICTION_POLICY_RANDOM,
} eviction_policy_t;

typedef enum {
    BUFFER_ACCESS_TYPE_RD,
    BUFFER_ACCESS_TYPE_WR,
} access_type_t;
