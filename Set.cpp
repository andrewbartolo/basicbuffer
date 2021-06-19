/*
 * Implementation of an individual cache set, with N ways.
 *
 * NOTE: currently, we use runtime logic to bifurcate between the various
 * eviction policies, which have different implementations. In the future, it
 * may be preferable to, e.g., split into LRUSet and RandomSet to avoid some
 * runtime overhead.
 */
#include "Set.h"



Set::Set(size_t gid, allocation_policy_t allocation_policy, eviction_policy_t
        eviction_policy, size_t n_ways) : gid(gid),
        allocation_policy(allocation_policy), eviction_policy(eviction_policy),
        n_ways(n_ways), rand_gen(gid), rand_dist(0, n_ways - 1)
{
    if (eviction_policy == BUFFER_EVICTION_POLICY_LRU) {

    }
    else if (eviction_policy == BUFFER_EVICTION_POLICY_RANDOM) {
        // reserve some space in the vector
        rand_vec.reserve(n_ways);
        // RAII: PRNG and dist already initialized in initializer list
    }
    else { }

    n_ways_active = 0;
}


void
Set::access(line_addr_t line_addr, access_type_t type, bool* was_hit)
{
    if (eviction_policy == BUFFER_EVICTION_POLICY_LRU) {
        // is it a hit?
        auto it = lru_map.find(line_addr);
        if (it != lru_map.end()) {
            // it was a hit
            *was_hit = true;
            (type == BUFFER_ACCESS_TYPE_RD) ? ++n_rd_hits : ++n_wr_hits;

            // reset the last-used time by removing and appending
            lru_list.erase(it->second);
            auto new_it = lru_list.emplace(lru_list.end(), line_addr);
            lru_map[line_addr] = new_it;
        }
        else {
            // it was a miss
            *was_hit = false;
            (type == BUFFER_ACCESS_TYPE_RD) ? ++n_rd_misses : ++n_wr_misses;

            // based on our allocation policy, do we want to allocate or not?
            bool do_allocate = (type == BUFFER_ACCESS_TYPE_WR) or
                    ((type == BUFFER_ACCESS_TYPE_RD)
                    and (allocation_policy == BUFFER_ALLOCATION_POLICY_AORW));

            if (do_allocate) {
                if (n_ways_active == n_ways) {
                    // need to evict least-recent element (map erase first!)
                    auto to_evict = lru_list.begin();
                    lru_map.erase(*to_evict);
                    lru_list.erase(to_evict);

                    // insert the new element
                    auto new_it = lru_list.emplace(lru_list.end(), line_addr);
                    lru_map[line_addr] = new_it;

                    ++n_evictions;
                }
                else {
                    // don't need to evict
                    // insert the new element
                    auto new_it = lru_list.emplace(lru_list.end(), line_addr);
                    lru_map[line_addr] = new_it;

                    ++n_ways_active;
                }
            }
        }
    }


    else if (eviction_policy == BUFFER_EVICTION_POLICY_RANDOM) {
        // is it a hit?
        auto it = rand_set.find(line_addr);
        if (it != rand_set.end()) {
            // it was a hit
            *was_hit = true;
            (type == BUFFER_ACCESS_TYPE_RD) ? ++n_rd_hits : ++n_wr_hits;
        }
        else {
            // it was a miss
            *was_hit = false;
            (type == BUFFER_ACCESS_TYPE_RD) ? ++n_rd_misses : ++n_wr_misses;

            // based on our allocation policy, do we want to allocate or not?
            bool do_allocate = (type == BUFFER_ACCESS_TYPE_WR) or
                    ((type == BUFFER_ACCESS_TYPE_RD)
                    and (allocation_policy == BUFFER_ALLOCATION_POLICY_AORW));

            if (do_allocate) {
                if (n_ways_active == n_ways) {
                    // choose a random element to evict
                    size_t vec_idx = rand_dist(rand_gen);
                    line_addr_t to_evict = rand_vec[vec_idx];
                    rand_vec[vec_idx] = line_addr;

                    rand_set.erase(to_evict);
                    rand_set.emplace(line_addr);

                    ++n_evictions;
                }
                else {
                    // append to the vector
                    rand_vec.emplace_back(line_addr);
                    rand_set.emplace(line_addr);

                    ++n_ways_active;
                }
            }
        }
    }

    else { }
}
