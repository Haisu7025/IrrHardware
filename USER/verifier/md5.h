#ifndef __MD5_H
#define __MD5_H

#include "sys.h"

typedef struct
{
    u32 count[2];
    u32 state[4];
    unsigned char buffer[64];
}MD5_CTX;


#define F(x,y,z) ((x & y) | (~x & z))
#define G(x,y,z) ((x & z) | (y & ~z))
#define H(x,y,z) (x^y^z)
#define I(x,y,z) (y ^ (x | ~z))
#define ROTATE_LEFT(x,n) ((x << n) | (x >> (32-n)))
#define FF(a,b,c,d,x,s,ac) \
          { \
          a += F(b,c,d) + x + ac; \
          a = ROTATE_LEFT(a,s); \
          a += b; \
          }
#define GG(a,b,c,d,x,s,ac) \
          { \
          a += G(b,c,d) + x + ac; \
          a = ROTATE_LEFT(a,s); \
          a += b; \
          }
#define HH(a,b,c,d,x,s,ac) \
          { \
          a += H(b,c,d) + x + ac; \
          a = ROTATE_LEFT(a,s); \
          a += b; \
          }
#define II(a,b,c,d,x,s,ac) \
          { \
          a += I(b,c,d) + x + ac; \
          a = ROTATE_LEFT(a,s); \
          a += b; \
          }
void MD5Init(MD5_CTX *context);
void MD5Update(MD5_CTX *context,unsigned char *input,u32 inputlen);
void MD5Final(MD5_CTX *context,unsigned char digest[16]);
void MD5Transform(u32 state[4],unsigned char block[64]);
void MD5Encode(unsigned char *output,u32 *input,u32 len);
void MD5Decode(u32 *output,unsigned char *input,u32 len);

#endif
