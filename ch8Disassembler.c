//Ryan Patrick

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void decodeInstruction(char *machineCode, int pC);

int main()
{
    FILE * rom = fopen("test.ch8", "rb");

    //Get file size for memory allocation
    fseek(rom, 0, SEEK_END);
    long codeSize = ftell(rom);
    fseek(rom, 0, SEEK_SET);

    char *machineCode = malloc(sizeof(0x200 + codeSize));
    fread(0x200 + machineCode, codeSize, 1, rom); // Read the rom in at 0x200
    fclose(rom);

    long memorySize = 0x200 + codeSize;
    int pC = 0x200;

    printf("ADDR | Instruction\n\n");

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
                //SYS

                //CLS

                //RET
                break;
            }
        case 0x1:
            {
                // 0000 0000 0000 0000
                // 1111
                // 0000 1111 0000 0000 | byte[1] 1010 1110

                uint16_t address = byte[0] << 12;
                address = address >> 4;
                address = address | byte[1];

                printf("%04d   JP %d\n", pC, address);

                // Jump instruction - 1nnn - JP addr, where addr = nnn
                break;
            }
        case 0x2:
            {
                // 2nnn - CALL addr, where addr = nnn
                break;
            }
        case 0x3:
            {
                uint8_t reg = byte[0] << 4;
                reg = reg >> 4;

                uint8_t val = byte[1];

                printf("%04d   SE V%d, %d\n", pC, reg, val);

                break;
            }
        case 0x4:
            {
                uint8_t reg = byte[0] << 4;
                reg = reg >> 4;

                uint8_t val = byte[1];

                printf("%04d   SNE V%d, %d\n", pC, reg, val);

                break;
            }
        case 0x5:
            {
                uint8_t regX = byte[0] << 4;
                regX = regX >> 4;
                uint8_t regY = byte[1] >> 4;

                printf("%04d   SE V%d, V%d\n", pC, regX, regY);

                break;
            }
        case 0x6:
            {
                uint8_t val = byte[1];
                uint8_t reg = byte[0] << 4;
                reg = reg >> 4;

                printf("%04d   LD V%d, %d\n", pC, reg, val);

                break;
            }
        case 0x7:
            {
                uint8_t val = byte[1];
                uint8_t reg = byte[0] << 4;
                reg = reg >> 4;

                printf("%04d   ADD V%d, %d\n", pC, reg, val);

                break;
            }
        case 0x8:
            {
                uint8_t regX = byte[0] << 4;
                regX = regX >> 4;
                uint8_t regY = byte[1] >> 4;

                halfByte = byte[1] << 4;
                halfByte = halfByte >> 4;

                switch(halfByte)
                {
                    case 0x0:
                    {
                        printf("%04d   LD V%d, V%d\n", pC, regX, regY);
                        break;
                    }
                    case 0x1:
                    {
                        printf("%04d   OR V%d, V%d\n", pC, regX, regY);
                        break;
                    }
                    case 0x2:
                    {
                        printf("%04d   AND V%d, V%d\n", pC, regX, regY);
                        break;
                    }
                    case 0x3:
                    {
                        printf("%04d   XOR V%d, V%d\n", pC, regX, regY);
                        break;
                    }
                    case 0x4:
                    {
                        printf("%04d   ADD V%d, V%d\n", pC, regX, regY);
                        break;
                    }
                    case 0x5:
                    {
                        printf("%04d   SUB V%d, V%d\n", pC, regX, regY);
                        break;
                    }
                    case 0x6:
                    {
                        printf("%04d   SHR V%d {, V%d}\n", pC, regX, regY);
                        break;
                    }
                    case 0x7:
                    {
                        printf("%04d    SUBN V%d, V%d\n", pC, regX, regY);
                        break;
                    }
                    case 0xe:
                    {
                        printf("%04d   SHL V%d {, V%d}\n", pC, regX, regY);
                        break;
                    }
                }

                break;
            }
        case 0x9:
            {
                uint8_t regX = byte[0] << 4;
                regX = regX >> 4;

                uint8_t regY = byte[1] >> 4;

                printf("%04d   SNE V%d, V%d\n", pC, regX, regY);

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
                uint8_t reg = byte[0] << 4;
                reg = reg >> 4;

                uint8_t val = byte[1];

                printf("%04d   RND V%d, %d\n", pC, reg, val);

                break;
            }
        case 0xd:
            {
                uint8_t regX = byte[0] << 4;
                regX = regX >> 4;

                uint8_t regY = byte[1] >> 4;
                uint8_t nibble = byte[1] << 4;
                nibble = nibble >> 4;

                printf("%04d   DRW V%d, V%d, %d\n", pC, regX, regY, nibble);

                break;
            }
        case 0xe:
            {
                uint8_t reg = byte[0] << 4;
                reg = reg >> 4;

                uint8_t fullByte = byte[1];

                switch(fullByte)
                {
                    case 0x9e:
                    {
                        printf("%04d   SKP V%d\n", pC, reg);
                        break;
                    }
                    case 0xa1:
                    {
                        printf("%04d   SKNP V%d\n", pC, reg);
                        break;
                    }
                }

                break;
            }
        case 0xf:
            {
                uint8_t reg = byte[0] << 4;
                reg = reg >> 4;

                uint8_t fullByte = byte[1];

                switch(fullByte)
                {
                    case 0x07:
                    {
                        printf("%04d   LD V%d, DT\n", pC, reg);
                        break;
                    }
                    case 0x0a:
                    {
                        printf("%04d   LD V%d, K\n", pC, reg);
                        break;
                    }
                    case 0x15:
                    {
                        printf("%04d   LD DT, V%d\n", pC, reg);
                        break;
                    }
                    case 0x18:
                    {
                        printf("%04d   LD ST, V%d\n", pC, reg);
                        break;
                    }
                    case 0x1e:
                    {
                        printf("%04d   ADD I, V%d\n", pC, reg);
                        break;
                    }
                    case 0x29:
                    {
                        printf("%04d   LD F, V%d\n", pC, reg);
                        break;
                    }
                    case 0x33:
                    {
                        printf("%04d   LD B, V%d\n", pC, reg);
                        break;
                    }
                    case 0x55:
                    {
                        printf("%04d   LD [I], V%d\n", pC, reg);
                        break;
                    }
                    case 0x65:
                    {
                        printf("%04d   LD V%d, [I]\n", pC, reg);
                        break;
                    }
                }

                break;
            }
    }

    return;
}
