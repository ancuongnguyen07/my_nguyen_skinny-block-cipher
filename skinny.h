/**
 * Implement the following API.
 * You can add your own functions above, but don't modify below this line.
 */

/**
 * SKINNY-128-384 block cipher encryption.
 * Under 48-byte tweakey at k, encrypt 16-byte plaintext at p and store the 16-byte output at c.
 */
void single_bit(unsigned char* bit, unsigned char* byte, unsigned int n);
void subCells(unsigned char* initState);
void addConst(unsigned char* initState, unsigned int round);
void permutation(unsigned char* original, const unsigned int* permu, unsigned int n);
void LFSR_TK(unsigned char* tk, unsigned int i);
void addRTkey(unsigned char* initState, unsigned char* tk);
void shiftRows(unsigned char* initState);
void mixCols(unsigned char* initState);
void skinny(unsigned char *c, const unsigned char *p, const unsigned char *k);
