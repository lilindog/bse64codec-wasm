#ifndef _BASE64_H_
#define _BASE64_H_ 1

#define MAGICK_STR "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
// extern char* magick_str;
void (*base64_prehandle_reverse_map)(void);
unsigned char* base64_encode (unsigned char*, unsigned char*);
unsigned char* base64_decode (unsigned char*, unsigned char*);

#endif