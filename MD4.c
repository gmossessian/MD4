
/*
 * MD4.c
 *
 *  Created on: Dec 21, 2015
 *      Author: George Mossessian
 *
 *      The MD4 hash algorithm, as described in https://tools.ietf.org/html/rfc1320
 */

#include "MD4.h"


#define F(X,Y,Z) (((X)&(Y))|((~(X))&(Z)))
#define G(X,Y,Z) (((X)&(Y))|((X)&(Z))|((Y)&(Z)))
#define H(X,Y,Z) ((X)^(Y)^(Z))

#define LEFTROTATE(A,N) ((A)<<(N))|((A)>>(32-(N)))

#define MD4ROUND1(a,b,c,d,x,s) a += F(b,c,d) + x; a = LEFTROTATE(a, s);
#define MD4ROUND2(a,b,c,d,x,s) a += G(b,c,d) + x + (uint32_t)0x5A827999; a = LEFTROTATE(a, s);
#define MD4ROUND3(a,b,c,d,x,s) a += H(b,c,d) + x + (uint32_t)0x6ED9EBA1; a = LEFTROTATE(a, s);

void resetMD4Registers(void);

uint32_t A = 0x67452301;
uint32_t B = 0xefcdab89;
uint32_t C = 0x98badcfe;
uint32_t D = 0x10325476;

string MD4(string message){
	string m=LOCALSTRING(message);
	string digest;
	uint32_t *w;
	uint32_t *hash;
	uint64_t mlen=m.len;
	unsigned char oneBit = 0x80;
	int i, wlen;


	m=stringCat(m, newString((char *)&oneBit,1));

	//append 0 ≤ k < 512 bits '0', such that the resulting message length in bits
	//	is congruent to −64 ≡ 448 (mod 512)4
	i=((56-m.len)%64);
	if(i<0) i+=64;
	m=stringCat(m,newString(NULL, i));

	w = malloc(sizeof(uint32_t)*(m.len/4+2));

	//append length, in bits (hence <<3), least significant word first
	for(i=0; i<m.len/4; i++){
		w[i]=stringToUint32(newString(&(m.c[4*i]), 4));
	}
	w[i++] = (mlen<<3) & 0xFFFFFFFF;
	w[i++] = (mlen>>29) & 0xFFFFFFFF;

	wlen=i;


	//change endianness, but not for the appended message length, for some reason?
	for(i=0; i<wlen-2; i++){
		w[i]=changeEndianness(w[i]);
	}

	hash = MD4Digest(w,wlen);

	digest=NULLSTRING;
	for(i=0; i<4; i++){
		hash[i]=changeEndianness(hash[i]);
		digest=stringCat(digest,uint32ToString(hash[i]));
	}

	return digest;
}

uint32_t *MD4Digest(uint32_t *w, int len){
	//assumes message.len is a multiple of 64 bytes.
	int i,j;
	uint32_t X[16];
	uint32_t *digest = malloc(sizeof(uint32_t)*4);
	uint32_t AA, BB, CC, DD;

	for(i=0; i<len/16; i++){
		for(j=0; j<16; j++){
			X[j]=w[i*16+j];
		}

		AA=A;
		BB=B;
		CC=C;
		DD=D;

		MD4ROUND1(A,B,C,D,X[0],3);
		MD4ROUND1(D,A,B,C,X[1],7);
		MD4ROUND1(C,D,A,B,X[2],11);
		MD4ROUND1(B,C,D,A,X[3],19);
		MD4ROUND1(A,B,C,D,X[4],3);
		MD4ROUND1(D,A,B,C,X[5],7);
		MD4ROUND1(C,D,A,B,X[6],11);
		MD4ROUND1(B,C,D,A,X[7],19);
		MD4ROUND1(A,B,C,D,X[8],3);
		MD4ROUND1(D,A,B,C,X[9],7);
		MD4ROUND1(C,D,A,B,X[10],11);
		MD4ROUND1(B,C,D,A,X[11],19);
		MD4ROUND1(A,B,C,D,X[12],3);
		MD4ROUND1(D,A,B,C,X[13],7);
		MD4ROUND1(C,D,A,B,X[14],11);
		MD4ROUND1(B,C,D,A,X[15],19);

		MD4ROUND2(A,B,C,D,X[0],3);
		MD4ROUND2(D,A,B,C,X[4],5);
		MD4ROUND2(C,D,A,B,X[8],9);
		MD4ROUND2(B,C,D,A,X[12],13);
		MD4ROUND2(A,B,C,D,X[1],3);
		MD4ROUND2(D,A,B,C,X[5],5);
		MD4ROUND2(C,D,A,B,X[9],9);
		MD4ROUND2(B,C,D,A,X[13],13);
		MD4ROUND2(A,B,C,D,X[2],3);
		MD4ROUND2(D,A,B,C,X[6],5);
		MD4ROUND2(C,D,A,B,X[10],9);
		MD4ROUND2(B,C,D,A,X[14],13);
		MD4ROUND2(A,B,C,D,X[3],3);
		MD4ROUND2(D,A,B,C,X[7],5);
		MD4ROUND2(C,D,A,B,X[11],9);
		MD4ROUND2(B,C,D,A,X[15],13);

		MD4ROUND3(A,B,C,D,X[0],3);
		MD4ROUND3(D,A,B,C,X[8],9);
		MD4ROUND3(C,D,A,B,X[4],11);
		MD4ROUND3(B,C,D,A,X[12],15);
		MD4ROUND3(A,B,C,D,X[2],3);
		MD4ROUND3(D,A,B,C,X[10],9);
		MD4ROUND3(C,D,A,B,X[6],11);
		MD4ROUND3(B,C,D,A,X[14],15);
		MD4ROUND3(A,B,C,D,X[1],3);
		MD4ROUND3(D,A,B,C,X[9],9);
		MD4ROUND3(C,D,A,B,X[5],11);
		MD4ROUND3(B,C,D,A,X[13],15);
		MD4ROUND3(A,B,C,D,X[3],3);
		MD4ROUND3(D,A,B,C,X[11],9);
		MD4ROUND3(C,D,A,B,X[7],11);
		MD4ROUND3(B,C,D,A,X[15],15);

		A+=AA;
		B+=BB;
		C+=CC;
		D+=DD;
	}

	digest[0]=A;
	digest[1]=B;
	digest[2]=C;
	digest[3]=D;
	resetMD4Registers();
	return digest;
}

uint32_t changeEndianness(uint32_t x){
	return ((x & 0xFF) << 24) | ((x & 0xFF00) << 8) | ((x & 0xFF0000) >> 8) | ((x & 0xFF000000) >> 24);
}

void setMD4Registers(uint32_t AA, uint32_t BB, uint32_t CC, uint32_t DD){
	A=AA;
	B=BB;
	C=CC;
	D=DD;
}

void resetMD4Registers(void){
	setMD4Registers(0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476);
}
