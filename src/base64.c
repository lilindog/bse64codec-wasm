#include "string.h"
#include "wasm.h"
#define MAGICK_STR "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"

static unsigned char* magick_str = (unsigned char*)MAGICK_STR;

// 直接能装下整个ascii（包括eascii）
static unsigned char reverse_map[255];

static int _prehandle_reverse_map_inited = 0;
static void prehandle_reverse_map () 
{
    if (_prehandle_reverse_map_inited) return;
    _prehandle_reverse_map_inited = 1;
    for (int i = 0; i < _strlen(magick_str); i++)
    {
        reverse_map[magick_str[i]] = i;
    }
}

void (*base64_prehandle_reverse_map)(void) = &prehandle_reverse_map;

EXPORT_FUNC
unsigned int base64_encode (unsigned char* bytes, unsigned int byteLen, unsigned char* dest) 
{
    unsigned long i = 0;
    unsigned long j = 0;

    while (i < byteLen) {;
        unsigned long left = byteLen - i;
        unsigned int a, b, c, d;
        
        // 末尾只剩2byte
        if (left == 2) 
        {
            a = bytes[i] >> 2;
            b = ((bytes[i] & 0x03) << 4) | (bytes[i + 1] >> 4);
            c = (bytes[i + 1] & 0x0f) << 2;

            dest[j++] = magick_str[a];
            dest[j++] = magick_str[b];
            dest[j++] = magick_str[c];
            dest[j++] = '=';
            dest[j] = '\0';
        }
        // 末尾只剩1byte
        else if (left == 1) 
        {
            a = bytes[i] >> 2;
            b = (bytes[i] & 0x03) << 4;

            dest[j++] = magick_str[a];
            dest[j++] = magick_str[b];
            dest[j++] = '=';
            dest[j++] = '=';
            dest[j] = '\0';
        }
        // 足byte
        else 
        {
            a = bytes[i] >> 2;
            b = ((bytes[i] & 0x03) << 4) | (bytes[i + 1] >> 4);
            c = ((bytes[i + 1] & 0x0f) << 2) | (bytes[i + 2] >> 6);
            d = bytes[i + 2] & 0x3f;

            dest[j++] = magick_str[a];
            dest[j++] = magick_str[b];
            dest[j++] = magick_str[c];
            dest[j++] = magick_str[d];
            if (left == 3) 
            {
                dest[j] = '\0';
            }
        }
        i += 3;
    }

    return j;
}

EXPORT_FUNC
unsigned int base64_decode (unsigned char* bytes, unsigned int byteLen, char* dest)
{
    prehandle_reverse_map();
    unsigned long i = 0;
    unsigned long j = 0;
   
    // 数量一定是4的整数倍
    // 这里暂时不考虑刻意省略pad
    while (i < byteLen) 
    {
        char a = reverse_map[bytes[i++]];
        char b = reverse_map[bytes[i++]];
        char c = -1;
        char d = -1;
        
        if (bytes[i] != '=') 
            c = reverse_map[bytes[i++]];
        else 
            i++;
        if (bytes[i] != '=')
            d = reverse_map[bytes[i++]];
        else
            i++;
        
        // byte 1
        dest[j++] = (a << 2) | ((b >> 4) & 0x03);

        if (c != -1) 
            // byte 2
            dest[j++] = ((b & 0x0f) << 4) | ((c >> 2) & 0x0f);
        if (d != -1)
            // byte 3
            dest[j++] = ((c & 0x03) << 6) | (d & 0x3f);
    }

    dest[j] = '\0';

    return j;
}