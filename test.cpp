#include <stdio.h>

#include "defs.h"
#include "Buffer.h"


// just use #defines for testing
#define ALLOCATION_POLICY BUFFER_ALLOCATION_POLICY_AORW
#define EVICTION_POLICY BUFFER_EVICTION_POLICY_LRU
#define LINE_SIZE 64
#define N_LINES 131072
#define N_BANKS 4
#define N_WAYS 16



int
main(int argc, char* argv[])
{
    fprintf(stderr, "testing buffer...\n");

    Buffer b(ALLOCATION_POLICY, EVICTION_POLICY, LINE_SIZE, N_LINES, N_BANKS,
            N_WAYS);


    /* test */
    bool was_hit;
    b.access((addr_t) 0x01234567, BUFFER_ACCESS_TYPE_RD, &was_hit);
    b.access((addr_t) 0x01234567, BUFFER_ACCESS_TYPE_RD, &was_hit);


    b.aggregate_stats();
    b.print_stats();


    fprintf(stderr, "done.\n");
    return 0;
}
