void skinny(unsigned char *c, const unsigned char *p, const unsigned char *k) {
    // it gets more complicated if I made the array in 4x4 form so 1x16 it is.
    unsigned char IS[16];
    unsigned char TK[48];

    unsigned char* LFSR = {0,0,0,0,0,0};
    for (unsigned int i = 0; i <  16; i++)
    {
        IS[i] = p[i];
    }
    

    for (unsigned int i = 0; i < 48; i++)
    {
        TK[i] = k[i];
    }

    // one of 56 rounds of encryption
    for (unsigned int r = 0; r < ROUND; r++)
    {
        subCells(IS);
        addConst(IS,r,LFSR);
        addRTkey(IS,TK);
        shiftRows(IS);
        mixCols(IS);
    }
    
}


const unsigned int* SUBCELLS_P[] = {2,1,7,6,4,0,3,5};
const unsigned int* SUBCELLS_PLAST[] = {7,6,5,4,3,1,2,0};

unsigned char* subCells(unsigned char* initState)
{
    // 4 iterations of SubCells
    for (unsigned int round = 0; round < 4; i++)
    {
        for (unsigned int cell = 0; cell < 16; cell++)
        {            
            unsigned char *byte = &is[cell];
            // Transformation of x4 and x0
            for (unsigned int i = 0; i < 5; i + 4)
            {
                unsigned char x,y,z;
                single_bit(&x, byte, i);
                single_bit(&y, byte, i + 2);
                single_bit(&z, byte, i + 3);
                
                *(byte + i) = x ^ (~(y | z));
                
            }

            // bit permutation
            unsigned int* order = SUBCELLS_P;
            if (round == 3)
            {
                order = SUBCELLS_PLAST;
            }           
            unsigned char permutedByte[8];
            for (unsigned int i = 0; i < 8; i++)
            {
                permutedByte[i] = byte[order[i];]
            }
            *byte = permutedByte;
        }       
    }
    
}

void update_LFSR(unsigned char* LFSR)
{
        // take out rc4, rc5
        unsigned char* rc4, rc5 newrc0;
        single_bit(rc4, LFSR, 5);
        single_bit(rc5, LFSR, 6);
        newrc0 = (*rc4 ^ *rc5 ^ 1) << 2;
        newrc0[6] = NULL;
        newrc0[7] = NULL;
        // how do I convert this into 6 bit state?

        unsigned char* constState[6];
        for ( unsigned int i = 0; i < 6; i++)
        {
            *constState[i] = (*LFSR[i] << 1 );
        }
        LFSR = constState ^ newrc0;
}
void* addConst(unsigned char* initState, int* round, unsigned char* LFSR)
{
    if ( round != 0)
    {
        update_LFSR(LFSR);
    }
    

    // XOR constants with some cells of IS
    std::vector<int*> Constants;
    unsigned char* c0, c1, c2;
    unsigned char* zeros = 0x00;
    *c0 = zeros ^ ;
    *c1 = zeros ^;
    *c2 = 0x02;
    Constants += {c0,c1,c2};

    for ( unsigned int i = 0; i < 3; i++)
    {
        unsigned int pos = 4*i + i;
        initState[pos] = initState[pos] ^ Constants[i];
    }   
}
