//Ryan Patrick

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void decodeInstruction(char *machineCode, int pC);

int main()
{
    FILE * rom = fopen("RNT.ch8", "rb");

    //Get file size for memory allocation
    fseek(rom, 0, SEEK_END);
    long codeSize = ftell(rom);
    fseek(rom, 0, SEEK_SET);

    char *machineCode = malloc(sizeof(0x200 + codeSize));
    fread(0x200 + machineCode, codeSize, 1, rom); // Read the rom in at 0x200
    fclose(rom);

    long memorySize = 0x200 + codeSize;
    int pC = 0x200;

    while(pC < memorySize)
    {
        //printf("pC = %d\n", pC);

        decodeInstruction(machineCode, pC);
        pC += 2;
    }

    // Free the memory allocated
    free(machineCode);

    return 0;
}


void decodeInstruction(char *machineCode, int pC)
{
    uint8_t *byte = &machineCode[pC];
    uint8_t halfByte = byte[0] >> 4; // Bitshift 4 bits right so we only look at first 4 bits

    //printf("%d\n", halfByte);

    switch(halfByte)
    {
        case 0x0:
            {
                break;
            }
        case 0x1:
            {
                break;
            }
        case 0x2:
            {
                break;
            }
        case 0x3:
            {
                break;
            }
        case 0x4:
            {
                break;
            }
        case 0x5:
            {
                break;
            }
        case 0x6:
            {
                break;
            }
        case 0x7:
            {
                break;
            }
        case 0x8:
            {
                break;
            }
        case 0x9:
            {
                break;
            }
        case 0xa:
            {
                break;
            }
        case 0xb:
            {
                break;
            }
        case 0xc:
            {
                break;
            }
        case 0xd:
            {
                break;
            }
        case 0xe:
            {
                break;
            }
        case 0xf:
            {
                break;
            }
    }

}
