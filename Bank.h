/*
 * A Bank in the buffer (simulate being accessed in parallel).
 */
#pragma once

#include <stddef.h>
#include <stdint.h>

#include <vector>

#include "defs.h"
#include "Set.h"


class Bank {
    public:
        Bank(size_t gid, allocation_policy_t allocation_policy,
                eviction_policy_t eviction_policy, size_t n_sets,
                size_t n_ways);

        void access(line_addr_t line_addr, access_type_t type, bool* was_hit);

        // Buffer iterates over these in aggregate_stats()
        std::vector<Set> sets;

    private:
        size_t gid;
        size_t n_sets;
        size_t n_ways;
};
