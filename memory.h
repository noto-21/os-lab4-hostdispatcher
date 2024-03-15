#include <stdio.h>
#include <stdlib.h>
#include "resource.h"

#ifndef MEMORY_H_
#define MEMORY_H_

resource_t *available_res;

const MEM_MODE_REALTIME = 0;
const MEM_MODE_JOB = 1;

enum MEM_MODE {
    REALTIME = MEM_MODE_REALTIME,
    JOB = MEM_MODE_JOB
};

// Return memoy index
extern int allocate_memory(enum MEM_MODE mode, int mbytes);

#endif /* MEMORY_H_ */