#include "Bank.h"

Bank::Bank(size_t gid, allocation_policy_t allocation_policy, eviction_policy_t
        eviction_policy, size_t n_sets, size_t n_ways) : gid(gid),
        n_sets(n_sets), n_ways(n_ways)
{

    for (size_t i = 0; i < n_sets; ++i) {
        size_t set_gid = gid * n_sets + i;
        // still invokes move constructor
        sets.emplace_back(set_gid, allocation_policy, eviction_policy, n_ways);
    }

}

void
Bank::access(line_addr_t line_addr, access_type_t type, bool* was_hit)
{
    // look up the correct set within the bank
    size_t set_idx = line_addr % n_sets;

    sets[set_idx].access(line_addr, type, was_hit);
}
