#include <assert.h>
#include <math.h>

#include "Buffer.h"
#include "util.h"


Buffer::Buffer(allocation_policy_t allocation_policy, eviction_policy_t
        eviction_policy, size_t line_size, size_t n_lines, size_t n_banks,
        size_t n_ways) : allocation_policy(allocation_policy),
        eviction_policy(eviction_policy), line_size(line_size),
        n_lines(n_lines), n_banks(n_banks), n_ways(n_ways)
{
    // for simplicity, all arguments must be a power of two
    assert(__builtin_popcount(line_size) == 1);
    assert(__builtin_popcount(n_lines) == 1);
    assert(__builtin_popcount(n_lines) == 1);
    assert(__builtin_popcount(n_banks) == 1);
    assert(__builtin_popcount(n_ways) == 1);


    line_size_log2 = log2(line_size);

    size_t n_lines_per_bank = n_lines / n_banks;
    size_t n_sets_per_bank = n_lines_per_bank / n_ways;

    // construct Banks
    for (size_t i = 0; i < n_banks; ++i) {
        size_t bank_gid = i;
        // still invokes move constructor
        banks.emplace_back(bank_gid, allocation_policy, eviction_policy,
                n_sets_per_bank, n_ways);
    }
}



void
Buffer::access(line_addr_t line_addr, access_type_t type, bool* was_hit)
{
    // hash to get a bank
    size_t bank_idx = fast_hash(line_addr, n_banks);

    banks[bank_idx].access(line_addr, type, was_hit);
}

void
Buffer::access(addr_t addr, access_type_t type, bool* was_hit)
{
    line_addr_t line_addr = addr >> line_size_log2;
    access(line_addr, type, was_hit);
}

void
Buffer::aggregate_stats()
{
    if (finalized) {
        fprintf(stderr, "ERROR: already called aggregate_stats()\n");
        return;
    }

    // for every Bank, for every Set within the Bank, aggregate its stats
    for (auto& b : banks) {
        for (auto& s : b.sets) {
            n_rd_hits +=    s.n_rd_hits;
            n_wr_hits +=    s.n_wr_hits; 
            n_rd_misses +=  s.n_rd_misses;
            n_wr_misses +=  s.n_wr_misses;
            n_evictions +=  s.n_evictions;
        }
    }


    finalized = true;
}

void
Buffer::print_stats()
{
    if (!finalized) {
        fprintf(stderr, "ERROR: must call aggregate_stats() before printing\n");
        return;
    }

    fprintf(stderr, "Read hits:\t\t%zu\n", n_rd_hits);
    fprintf(stderr, "Write hits:\t\t%zu\n", n_wr_hits);
    fprintf(stderr, "Read misses:\t\t%zu\n", n_rd_misses);
    fprintf(stderr, "Write misses:\t\t%zu\n", n_wr_misses);
    fprintf(stderr, "Evictions:\t\t%zu\n", n_evictions);
}
