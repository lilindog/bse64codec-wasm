#ifndef _MEMORY_H_
#define _MEMORY_H_

enum MemoryErrorType {
    MEMORY_COUNT_OVERFLOW = -1,
    MEMORY_OVERFLOW = -2,
    FREE_NOT_FOUND = -3,
};

int current_block_count;
int requestMemory(int);
int releaseMemory(int);

#endif