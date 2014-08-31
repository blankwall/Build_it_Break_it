// http://www.codeproject.com/Articles/35374/Two-Efficient-Classes-for-RC-and-Base-Stream-Ci

#include "rc4.h"

using namespace std;

XRC4::XRC4() {
    memset(inp_key, 0, 32);
	memset(sbox, 0, 256);
	memset(key, 0, 256);
}
XRC4::~XRC4() {
    memset(inp_key, 0, 32);
	memset(sbox, 0, 256);  /* remove Key traces in memory  */
	memset(key, 0, 256);
}

void XRC4::init(const char* k, size_t ksz) {
    inp_key_sz = ksz;
    memcpy(inp_key, k, ksz);
	i = 0, j = 0, n = 0;
    
	for (m = 0;  m < 256; m++)  /* Initialize the key sequence */
	{
		*(key + m)= *(inp_key + (m % inp_key_sz));
		*(sbox + m) = m;
	}
	for (m=0; m < 256; m++)
	{
		n = (n + *(sbox+m) + *(key + m)) &0xff;
		SWAP(*(sbox + m),*(sbox + n));
	}
}

char* XRC4::encrypt(char *pszText, size_t len) {

	for (m = 0; m < len; m++)
	{
		i = (i + 1) &0xff;
		j = (j + *(sbox + i)) &0xff;
		SWAP(*(sbox+i),*(sbox + j));  /* randomly Initialize
                                       the key sequence */
		k = *(sbox + ((*(sbox + i) + *(sbox + j)) &0xff ));
		*(pszText + m) ^= k;
	}
    
	return pszText;
}

char* XRC4::decrypt(char *pszText, size_t len) {
    return encrypt(pszText, len) ;  /* using the same function
                                       as encoding */
}
