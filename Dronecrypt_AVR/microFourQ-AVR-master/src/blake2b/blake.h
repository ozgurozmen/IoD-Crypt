#ifndef __BLAKE_H__
#define __BLAKE_H__


// For C++
#ifdef __cplusplus
extern "C" {
#endif


// Hashing using BLAKE2b. Output is 64 bytes long
int crypto_blake2b(const unsigned char *in, unsigned long long inlen, unsigned char *out);


#ifdef __cplusplus
}
#endif


#endif
