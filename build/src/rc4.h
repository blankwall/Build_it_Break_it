// http://www.codeproject.com/Articles/35374/Two-Efficient-Classes-for-RC-and-Base-Stream-Ci

#ifndef rc4_h
#define rc4_h

#include <string.h>
#include <iostream>

#define SWAP(a, b) ((a) ^= (b), (b) ^= (a), (a) ^= (b))

class XRC4 {
public:
    XRC4();
    ~XRC4();
    void init(const char* k, size_t ksz);
    char *encrypt(char *pszText, size_t len);
    char *decrypt(char *pszText, size_t len);
    
private:
    char inp_key[32];
    size_t inp_key_sz;
    unsigned char sbox[256];      /* Encryption array             */
    unsigned char key[256],k;     /* Numeric key values           */
    int  m, n, i, j;        /* Ambiguously named counters   */
};

#endif
