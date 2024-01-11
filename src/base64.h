#ifndef _BASE64_H_
#define _BASE64_H_

// extern char* magick_str;
void (*base64_prehandle_reverse_map)(void);
unsigned int base64_encode (unsigned char*, unsigned int, unsigned char*);
unsigned int base64_decode (unsigned char*, unsigned int, unsigned char*);

#endif