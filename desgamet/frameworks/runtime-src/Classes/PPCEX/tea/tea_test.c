#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "platform/CCPlatformConfig.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <WinSock.h>
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <arpa/inet.h>
#endif

//#define TRACEME(fmt,arg...) printf(fmt" in %s() at %s:%d.\n",##arg,__FUNCTION__,__FILE__,__LINE__)
 
void encrypt (uint32_t* v, uint32_t* k) {
    uint32_t v0=v[0], v1=v[1], sum=0, i;           /* set up */
    uint32_t delta=0x9e3779b9;                     /* a key schedule constant */
    uint32_t k0=k[0], k1=k[1], k2=k[2], k3=k[3];   /* cache key */
    for (i=0; i < 32; i++) {                       /* basic cycle start */
        sum += delta;
        v0 += ((v1<<4) + k0) ^ (v1 + sum) ^ ((v1>>5) + k1);
        v1 += ((v0<<4) + k2) ^ (v0 + sum) ^ ((v0>>5) + k3);  
    }                                              /* end cycle */
    v[0]=v0; v[1]=v1;}
 
void decrypt (uint32_t* v, uint32_t* k) {
    uint32_t v0=v[0], v1=v[1], sum=0xC6EF3720, i;  /* set up */
    uint32_t delta=0x9e3779b9;                     /* a key schedule constant */
    uint32_t k0=k[0], k1=k[1], k2=k[2], k3=k[3];   /* cache key */
    for (i=0; i<32; i++) {                         /* basic cycle start */
        v1 -= ((v0<<4) + k2) ^ (v0 + sum) ^ ((v0>>5) + k3);
        v0 -= ((v1<<4) + k0) ^ (v1 + sum) ^ ((v1>>5) + k1);
        sum -= delta;                                   
    }                                              /* end cycle */
    v[0]=v0; v[1]=v1;}

void dump(uint8_t* data, uint32_t size){
	return;
    printf("qq in ocean\n");
    int i;
    for(i=0;i<size;i++){
        printf("%02X ",data[i]);
        if(i%8==7) printf("%08X %08X\n",*(uint32_t*)(data+i-7),*(uint32_t*)(data+i-3));
    }
    printf("\n");
}

void encrypt16 (uint32_t* v, uint32_t* k) {
    uint32_t v0=v[0], v1=v[1], sum=0, i;           /* set up */
    uint32_t delta=0x9E3779B9;                     /* a key schedule constant */
    uint32_t k0=k[0], k1=k[1], k2=k[2], k3=k[3];   /* cache key */
    for (i=0; i < 16; i++) {                       /* basic cycle start */
        sum += delta;
        v0 += ((v1<<4) + k0) ^ (v1 + sum) ^ ((v1>>5) + k1);
        v1 += ((v0<<4) + k2) ^ (v0 + sum) ^ ((v0>>5) + k3);  
    }                                              /* end cycle */
    v[0]=v0; v[1]=v1;}
 
void decrypt16 (uint32_t* v, uint32_t* k) {
    uint32_t v0=v[0], v1=v[1], sum=0xE3779B90, i;  /* set up */
    uint32_t delta=0x9E3779B9;                     /* a key schedule constant */
    uint32_t k0=k[0], k1=k[1], k2=k[2], k3=k[3];   /* cache key */
    for (i=0; i<16; i++) {                         /* basic cycle start */
        v1 -= ((v0<<4) + k2) ^ (v0 + sum) ^ ((v0>>5) + k3);
        v0 -= ((v1<<4) + k0) ^ (v1 + sum) ^ ((v1>>5) + k1);
        sum -= delta;                                   
    }                                              /* end cycle */
    v[0]=v0; v[1]=v1;}
void qq_encrypt(uint8_t* plain,uint32_t size,uint32_t*key,uint32_t** crypt,uint32_t* c_size){
    //dump(plain,size);

    //add pad
    uint8_t random = rand();
    //printf("ran %x\n",random);

    uint8_t pad_size = 8 - (size + 10) % 8;
    pad_size = pad_size==8?0:pad_size;
	uint8_t* filled_data = (uint8_t*)malloc(size + 10 + pad_size);
    filled_data[0]=(random&0xf8)|pad_size;
    memset(filled_data+1,random&0xff,pad_size+2);
    memcpy(filled_data+pad_size+3,plain,size);
    memset(filled_data+pad_size+3+size,0,7);
    dump(filled_data,size+pad_size+10);

    uint32_t size32=(size+pad_size+10)/4;
    uint32_t* plain32 = (uint32_t*)filled_data;
    uint32_t* crypt32 = (uint32_t*)malloc(size+pad_size+10);
    memcpy(crypt32,plain32,size+pad_size+10);

    uint32_t i;
    uint32_t p_xor[2]={0x0,0x0};
    uint32_t c_xor[2]={0x0,0x0};
    for(i=0;i<size32;i+=2){
        crypt32[i] = htonl(crypt32[i]);
        crypt32[i+1] = htonl(crypt32[i+1]);
        //printf("%08X %08X ==> ",crypt32[i],crypt32[i+1]);
        crypt32[i] ^= p_xor[0];
        crypt32[i+1] ^= p_xor[1];
        encrypt16(crypt32+i,key);
        crypt32[i] ^= c_xor[0];
        crypt32[i+1] ^= c_xor[1];
        c_xor[0]=htonl(plain32[i]) ^ p_xor[0];
        c_xor[1]=htonl(plain32[i+1]) ^ p_xor[1];
        p_xor[0]=crypt32[i];
        p_xor[1]=crypt32[i+1];
        //printf("%08X %08X\n",crypt32[i],crypt32[i+1]);
        crypt32[i] = ntohl(crypt32[i]);
        crypt32[i+1] = ntohl(crypt32[i+1]);
    }
    //dump(crypt32,size+pad_size+10);
	free(filled_data);
    *crypt = crypt32;
    *c_size = size+pad_size+10;
}

void qq_decrypt(uint32_t* crypt, uint32_t size, uint32_t* key, uint8_t** plain, uint32_t* p_size, uint32_t* fm){
    uint32_t size32 = size/4;

	uint32_t* plain32 = (uint32_t*)malloc(size);
	fm = plain32;
    memcpy(plain32,crypt,size);

    uint32_t i;
    uint32_t p_xor[2]={0x0,0x0};
    uint32_t c_xor[2]={0x0,0x0};
    for(i=0;i<size32;i+=2){
        plain32[i] = htonl(plain32[i]);
        plain32[i+1] = htonl(plain32[i+1]);
        //printf("%08X %08X ==> ",plain32[i],plain32[i+1]);
        plain32[i] ^= c_xor[0];
        plain32[i+1] ^= c_xor[1];
        decrypt16(plain32+i,key);
        c_xor[0]=plain32[i];
        c_xor[1]=plain32[i+1];
        plain32[i] ^= p_xor[0];
        plain32[i+1] ^= p_xor[1];
        p_xor[0]=htonl(crypt[i]);
        p_xor[1]=htonl(crypt[i+1]);
        //printf("%08X %08X\n",plain32[i],plain32[i+1]);
        plain32[i] = ntohl(plain32[i]);
        plain32[i+1] = ntohl(plain32[i+1]);
    }
    dump((uint8_t*)plain32,size);

    //remove pad
    uint32_t pad_size = plain32[0]&0x7;
//    dump((uint8_t*)plain32+3+pad_size,size-10-pad_size);  

    *plain=(uint8_t*)plain32+3+pad_size;
    *p_size=size-10-pad_size;
}


/*
int main(){
    //uint32_t key[4]={0x76543298,0x7ab5429f,0x653bc89d,0x76529a6b};
    uint32_t key[4]={0x98325476,0x9f42b57a,0x9dc83b65,0x6b9a5276};
    uint32_t plain[2]={0x34876523,0x23765498};
    printf("%X %X\n",plain[0],plain[1]);
    encrypt(plain,key); 
    printf("%X %X\n",plain[0],plain[1]);
    decrypt(plain,key);
    printf("%X %X\n",plain[0],plain[1]);

    char plain_str[]="{cmd=reg,username=liqing,password=819cf1304065c4ae95f2babaf8a03fd7}";
    uint32_t* crypt;
    uint32_t size;
	qq_encrypt((uint8_t*)plain_str, strlen(plain_str), key, &crypt, &size);
    dump((uint8_t*)crypt,size);

    uint8_t* plain2;
    uint32_t p_size;
    qq_decrypt(crypt,size,key,&plain2,&p_size);
    dump(plain2,p_size);

    printf("********************************************************\n");
    uint8_t crypt_test[]={0xC5,0x66,0xD4,0xEC,0x31,0xBD,0x2F,0xE8,
                        0x00,0x32,0x5E,0x18,0x4B,0x9C,0x82,0x0E,
                        0xAE,0x69,0xA1,0x48,0x13,0x2A,0x4D,0x84,
                        0xC4,0x44,0x97,0x4A,0x32,0x6D,0x45,0xD5,
                        0x4A,0x77,0x25,0x0B,0x34,0x71,0xD3,0x2C,
                        0x13,0xEC,0x4A,0xF7,0x9E,0x4A,0x0E,0xAE,
                        0x7A,0xE1,0x8D,0x69,0xCC,0xD6,0x69,0xFD,
                        0x80,0xC4,0xF4,0x94,0xB5,0x91,0xC3,0x8F,
                        0xC7,0xEE,0x4C,0x3E,0xF9,0x5E,0x8E,0xA0,
                        0x38,0x5B,0xFD,0x44,0xF0,0xF0,0xC3,0x55};
    uint8_t* plain3;
    uint32_t p_size3;
	qq_decrypt((uint32_t*)crypt_test, 80, key, &plain3, &p_size3);
    dump(plain3,p_size3);

}
*/
    
