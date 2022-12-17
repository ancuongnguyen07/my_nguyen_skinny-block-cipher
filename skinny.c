#include <stdint.h>
#include <stdio.h>
#include "skinny.h"

/**
 * SKINNY-128-384 block cipher encryption.
 * Under 48-byte tweakey at k, encrypt 16-byte plaintext at p and store the 16-byte output at c.
 */
void skinny(unsigned char *c, const unsigned char *p, const unsigned char *k) {
    // it gets more complicated if I made the array in 4x4 form so 1x16 it is.
    unsigned char IS[16];
    unsigned char TK[48];

    

    for (unsigned int i = 0; i <  16; i++)
    {
        IS[i] = *(p+i);
    }
    

    for (unsigned int i = 0; i < 48; i++)
    {
        TK[i] = k[i];
    }

   
    // printf("assigned");

    // one of 56 rounds of encryption
    for (unsigned int r = 0; r < 56; r++)
    {   
        // printf("assigned");
        
        subCells(IS);

        
        addConst(IS,r);
        
        
        addRTkey(IS,TK);
        // printf("%02x", *(IS+2));
        
        shiftRows(IS);
        
    }
    
}

/* SKINNY Sbox */
const unsigned char S8[] = {
0x65 ,0x4c ,0x6a ,0x42 ,0x4b ,0x63 ,0x43 ,0x6b ,0x55 ,0x75 ,0x5a ,0x7a ,0x53 ,0x73 ,0x5b ,0x7b ,
0x35 ,0x8c ,0x3a ,0x81 ,0x89 ,0x33 ,0x80 ,0x3b ,0x95 ,0x25 ,0x98 ,0x2a ,0x90 ,0x23 ,0x99 ,0x2b ,
0xe5 ,0xcc ,0xe8 ,0xc1 ,0xc9 ,0xe0 ,0xc0 ,0xe9 ,0xd5 ,0xf5 ,0xd8 ,0xf8 ,0xd0 ,0xf0 ,0xd9 ,0xf9 ,
0xa5 ,0x1c ,0xa8 ,0x12 ,0x1b ,0xa0 ,0x13 ,0xa9 ,0x05 ,0xb5 ,0x0a ,0xb8 ,0x03 ,0xb0 ,0x0b ,0xb9 ,
0x32 ,0x88 ,0x3c ,0x85 ,0x8d ,0x34 ,0x84 ,0x3d ,0x91 ,0x22 ,0x9c ,0x2c ,0x94 ,0x24 ,0x9d ,0x2d ,
0x62 ,0x4a ,0x6c ,0x45 ,0x4d ,0x64 ,0x44 ,0x6d ,0x52 ,0x72 ,0x5c ,0x7c ,0x54 ,0x74 ,0x5d ,0x7d ,
0xa1 ,0x1a ,0xac ,0x15 ,0x1d ,0xa4 ,0x14 ,0xad ,0x02 ,0xb1 ,0x0c ,0xbc ,0x04 ,0xb4 ,0x0d ,0xbd ,
0xe1 ,0xc8 ,0xec ,0xc5 ,0xcd ,0xe4 ,0xc4 ,0xed ,0xd1 ,0xf1 ,0xdc ,0xfc ,0xd4 ,0xf4 ,0xdd ,0xfd ,
0x36 ,0x8e ,0x38 ,0x82 ,0x8b ,0x30 ,0x83 ,0x39 ,0x96 ,0x26 ,0x9a ,0x28 ,0x93 ,0x20 ,0x9b ,0x29 ,
0x66 ,0x4e ,0x68 ,0x41 ,0x49 ,0x60 ,0x40 ,0x69 ,0x56 ,0x76 ,0x58 ,0x78 ,0x50 ,0x70 ,0x59 ,0x79 ,
0xa6 ,0x1e ,0xaa ,0x11 ,0x19 ,0xa3 ,0x10 ,0xab ,0x06 ,0xb6 ,0x08 ,0xba ,0x00 ,0xb3 ,0x09 ,0xbb ,
0xe6 ,0xce ,0xea ,0xc2 ,0xcb ,0xe3 ,0xc3 ,0xeb ,0xd6 ,0xf6 ,0xda ,0xfa ,0xd3 ,0xf3 ,0xdb ,0xfb ,
0x31 ,0x8a ,0x3e ,0x86 ,0x8f ,0x37 ,0x87 ,0x3f ,0x92 ,0x21 ,0x9e ,0x2e ,0x97 ,0x27 ,0x9f ,0x2f ,
0x61 ,0x48 ,0x6e ,0x46 ,0x4f ,0x67 ,0x47 ,0x6f ,0x51 ,0x71 ,0x5e ,0x7e ,0x57 ,0x77 ,0x5f ,0x7f ,
0xa2 ,0x18 ,0xae ,0x16 ,0x1f ,0xa7 ,0x17 ,0xaf ,0x01 ,0xb2 ,0x0e ,0xbe ,0x07 ,0xb7 ,0x0f ,0xbf ,
0xe2 ,0xca ,0xee ,0xc6 ,0xcf ,0xe7 ,0xc7 ,0xef ,0xd2 ,0xf2 ,0xde ,0xfe ,0xd7 ,0xf7 ,0xdf ,0xff
};

// constants of LFSR in AddConstant function
const unsigned char CONSTANTS[] = 
{
    0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3e, 0x3d, 0x3b, 0x37, 0x2f, 0x1e, 0x3c, 0x39, 0x33, 0x27, 0x0e,
    0x1d, 0x3a, 0x35, 0x2b, 0x16, 0x2c, 0x18, 0x30, 0x21, 0x02, 0x05, 0x0b, 0x17, 0x2e, 0x1c, 0x38,
    0x31, 0x23, 0x06, 0x0d, 0x1b, 0x36, 0x2d, 0x1a, 0x34, 0x29, 0x12, 0x24, 0x08, 0x11, 0x22, 0x04,
    0x09, 0x13, 0x26, 0x0c, 0x19, 0x32, 0x25, 0x0a
};


// Give the byte that only contains desired bit at the end
void single_bit(unsigned char* bit, unsigned char* byte, unsigned int n)
{
    *bit = (*byte >> n) & 0x01;
}


void subCells(unsigned char* initState)
{
    
    for (unsigned int cell = 0; cell < 16; cell++)
        { 
            initState[cell] = S8[initState[cell]];
        }
}

void addConst(unsigned char* initState, unsigned int round)
{
    unsigned char c0 = CONSTANTS[round] & 0x0f;
    unsigned char c1 = CONSTANTS[round] >> 4;
    unsigned char c2 = 0x02; 

    initState[0] ^= c0;
    initState[4] ^= c1;
    initState[8] ^= c2; 
}

void permutation(unsigned char* original, const unsigned int* permu, unsigned int n)
{
    unsigned char copy[n];
    for (unsigned int i = 0; i < n; i++)
        { 
            copy[i] = original[permu[i]];
        }
    original = copy;
}

const unsigned int TK_PERMU[] = {9,15,8,13,10,14,12,11,0,1,2,3,4,5,6,7};
void LFSR_TK(unsigned char* tk, unsigned int i)
{
    unsigned char* byte;
    // if you declare here *xorb1 (*)
    unsigned char xorb1;
    unsigned char xorb2;
    
    // applying only on the first two rows of TK2 or TK3
    for (unsigned int row = 0; row < 2; row++)
    {
        for (unsigned int col = 0; col < 4; col++)
        {
            byte = tk + (4*row + col);

            if ( i == 2)
            {
                // (*) here you cannot call single_bit(xorb1, byte, 5)
                // as the pointer *xorb1 is uninitialized
                // the pointer is initialized if you assigned a value to it
                single_bit(&xorb1, byte, 5);
                single_bit(&xorb2, byte, 7);
                *byte = (*byte << 1) | (xorb1 ^ xorb2);
                
            }
            else if (i == 3)
            {
                single_bit(&xorb1, byte, 0);
                single_bit(&xorb2, byte, 6);

                // be careful here!!!
                // you need to replace x6 XOR x0 to the first bit
                // of the right-shifted bit string
                // For example, the original bit string: 1000 0011
                // x6 XOR x0: 0 XOR 1 = 1
                // right shift by 1: 0100 0001
                // what you need to do is:
                // 0100 0001
                // OR
                // 1000 0000
                // -> 1100 0001
                // BUT in C the OR (|) operation fills up the value
                // with leading 0s. For example, 1 -> 0000 0001
                // the when you do OR, it becomes
                // 0100 0001
                // OR
                // 0000 0001
                // Maybe I'am wrong :)). Just try to run and get back
                // here when error raised. Good luck!
                *byte = (*byte >> 1) | (xorb1 ^ xorb2); 
            }
        }
    }
    tk = byte;
}

void addRTkey(unsigned char* initState, unsigned char* tk)
{
    // printf("tk2");
    
    unsigned char* tk1 = tk;
    unsigned char* tk2 = tk + 16;
    
    
    unsigned char* tk3 = tk + 32;

    for (unsigned int i = 0; i < 2; i++)
    {
        for (unsigned int j = 0; j < 4; j++)
        {
            unsigned int pos = 4*i + j;
            initState[pos] ^= tk1[pos] ^ tk2[pos] ^ tk3[pos];
        }
    }

    
    
    // permute all tweakey arrays
    permutation(tk1, TK_PERMU,16);
    permutation(tk2, TK_PERMU,16);
    permutation(tk3, TK_PERMU,16);

    
    // LFSR on TK2, TK3
    LFSR_TK(tk2,2);    
    LFSR_TK(tk3,3);
}

const unsigned int SHIFTROWS_PERMUTATION[] = {0,1,2,3,7,4,5,6,10,11,8,9,13,14,15,12};
void shiftRows(unsigned char* initState)
{
    //printf("%02x", *(initState+2));
    permutation(initState, SHIFTROWS_PERMUTATION,16);
}