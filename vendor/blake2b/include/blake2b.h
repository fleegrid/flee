//
//  blake2b.h
//  flee
//
//  Created by Unit.2B on 2017/7/22.
//

#ifndef _BLAKE2B_H_
#define _BLAKE2B_H_

#define BLAKE2B_OUTBYTES 64
#define BLAKE2B_KEYBYTES 64

int blake2b(void *out, size_t outlen, const void *in, size_t inlen,
            const void *key, size_t keylen);

#endif /* _BLAKE2B_H_  */
