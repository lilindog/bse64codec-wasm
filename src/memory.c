#include "memory.h"
#include "wasm.h"
#include <stdio.h>

#define MEMORY_START 2048
#define MEMORY_BLOCK_COUNT 100
/* 暂定最大1.5GB的内存申请可能 */
#define MAX_MEMORY (1536 * 1024 * 1024)

typedef struct {
    int start;
    int length;
} MemoryBlock;

static MemoryBlock Memory[MEMORY_BLOCK_COUNT] = {};  

int current_block_count = 0;

static int _totalUseMemory () 
{
    int total = 0;
    for (int i = 0; i < current_block_count; i++)
    {
        total += (*(Memory + i)).length;
    }
    return total;
}

static void _insert (MemoryBlock node, int index) 
{
    for (int i = current_block_count; i >= 0; i--)
    {
        if (i == index)
        {
            Memory[i] = node;
            return;
        }
        Memory[i] = Memory[i - 1];
    }
}

// 申请到的内存块全部0填充，方便后续字符处理
static void _resetMemory (int start, int size) 
{
    int max = start + size;
    while (start < max)
    {
        *((unsigned char*)start) = 0;
        start++;
    }
}

EXPORT_FUNC
int requestMemory (int size)
{
    if (current_block_count + 1 > MEMORY_BLOCK_COUNT) 
    {
        return MEMORY_COUNT_OVERFLOW;
    }

    if (_totalUseMemory() >= MAX_MEMORY) 
    {
        return MEMORY_OVERFLOW;
    }

    int _size = size;
    while (_size % 8) _size++;

    MemoryBlock* prevNode = NULL;
    MemoryBlock* node = NULL;
    int maybeStart;

    // 寻找已分配间隙中是否有空间可分配
    for (int i = 0; i < current_block_count; i++)
    {
        node = &Memory[i];

        if (prevNode == NULL)
        {
            prevNode = node;
            continue;
        }

        maybeStart = prevNode->start + prevNode->length;
        int space = node->start - maybeStart;

        if (space >= _size)
        {
            MemoryBlock newNode = {
                .start = maybeStart,
                .length = _size,
            };
            _insert(newNode, i);
            node = NULL;
            prevNode = NULL;
            current_block_count++;

            _resetMemory(maybeStart, _size);
            return maybeStart;
        }

        prevNode = node;
    }

    // 间隙没有，末尾追加记录
    if (prevNode == NULL)
    {
        maybeStart = MEMORY_START + 0;
    } 
    else 
    {
        maybeStart = prevNode->start + prevNode->length;
    }
    MemoryBlock newNode = {
        .start = maybeStart,
        .length = _size,
    };
    _insert(newNode, current_block_count);
    current_block_count++;
    prevNode = NULL;

    _resetMemory(maybeStart, _size);
    return maybeStart;
}

EXPORT_FUNC
int releaseMemory (int start)
{
    int index = -1;
    int i;
    for (i = 0; i < current_block_count; i++)
    {
        MemoryBlock* node = &Memory[i];
        if (node->start == start)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        return FREE_NOT_FOUND;
    }

    for (i = index; i < current_block_count; i++)
    {
        Memory[i] = Memory[i + 1];
    }

    current_block_count--;

    return 1;
}

// ========= 以下辅助测试用 ==========

EXPORT_FUNC
int $getBlockCount ()
{
    return current_block_count;
}

EXPORT_FUNC
int $set (int start, unsigned int v)
{
    // *((unsigned int*)start) = v;
    return start;
}