//Ryan Patrick

#include <stdio.h>
#include <stdint.h>

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
        decodeInstruction(machineCode, pC);
        pC += 2;
    }

    return 0;
}


void decodeInstruction(char *machineCode, int pC)
{



}
