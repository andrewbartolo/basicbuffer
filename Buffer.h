/*
 * Top-level Buffer class.
 */
#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <vector>

#include "defs.h"
#include "Bank.h"


class Buffer {
    public:
        Buffer(allocation_policy_t allocation_policy, eviction_policy_t
                eviction_policy, size_t line_size, size_t n_lines,
                size_t n_banks, size_t n_ways);

        void access(line_addr_t line_addr, access_type_t type, bool* was_hit);
        void access(addr_t addr, access_type_t type, bool* was_hit);

        void aggregate_stats();
        void print_stats();

    private:
        allocation_policy_t allocation_policy;
        eviction_policy_t eviction_policy;
        size_t line_size;
        size_t n_lines;
        size_t n_banks;
        size_t n_ways;
        uint64_t line_size_log2;
        bool finalized = false;

        std::vector<Bank> banks;

        // aggregate statistics
        uint64_t n_rd_hits = 0;
        uint64_t n_wr_hits = 0;
        uint64_t n_rd_misses = 0;
        uint64_t n_wr_misses = 0;
        uint64_t n_evictions = 0;
};
