#ifndef __EMSCRIPTEN__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory.h"
#include "base64.h"

int main  (void)
{
    printf("start\n");

    char* str = "abcde";
    char* dest = (char*)malloc(1024 * 1024);

    char* de_dest = (char*)malloc(strlen(str));

    unsigned int len = base64_encode(str, strlen(str), dest);
    unsigned int delen = base64_decode(dest, len, de_dest);


    printf(">> %s\n", str);
    printf(">> len: %d\n", len);
    printf(">> %s\n", dest);

    printf(">d %d, %s\n", delen, de_dest);

    /*
    char* str = "abcdef";
    char* dest = (char*)malloc(100);
    char* decode_dest = (char*)malloc(100);

    base64_encode(str, dest);
    printf("base64 encode res: %s\n", dest);
    base64_decode(dest, decode_dest);
    printf("base64 edcode res: %s\n", decode_dest);
    */


    /*

    int p = _malloc(10000);
    printf("end: %d, count: %d\n", p, current_block_count);
    int p1 = _malloc(20000);
    printf("end1: %d, count: %d\n", p1, current_block_count);

    _free(p);
    printf("count: %d\n", current_block_count);
    _free(p1);
    printf("count: %d\n", current_block_count);

    int p2 = _malloc(100);
    printf("end2 %d, count: %d\n", p2, current_block_count);
    */
    
    
    return 0;
}

#endif  // #ifndef __EMSCRIPTEN__