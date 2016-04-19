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

    printf("ADDR | Instruction\n\n");

    while(pC < memorySize)
    {
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

    switch(halfByte)
    {
        case 0x0:
            {
                uint8_t fullByte = byte[0];

                if(fullByte == 0x0)
                {
                    fullByte = byte[1];

                    if(fullByte == 0xEE)
                    {
                        printf("%04x   RET\n", pC);
                    }
                    else
                    {
                        printf("%04x   CLS\n", pC);
                    }
                }
                else
                {
                    uint16_t address = byte[0] << 12;
                    address = address >> 4;
                    address = address | byte[1];

                    printf("%04x   SYS %x\n", pC, address);
                }

                break;
            }
        case 0x1:
            {
                uint16_t address = byte[0] << 12;
                address = address >> 4;
                address = address | byte[1];

                printf("%04x   JP %x\n", pC, address);

                break;
            }
        case 0x2:
            {
                uint16_t address = byte[0] << 12;
                address = address >> 4;
                address = address | byte[1];

                printf("%04x   CALL %x\n", pC, address);

                break;
            }
        case 0x3:
            {
                uint8_t reg = byte[0] << 4;
                reg = reg >> 4;

                uint8_t val = byte[1];

                printf("%04x   SE V%x, %d\n", pC, reg, val);

                break;
            }
        case 0x4:
            {
                uint8_t reg = byte[0] << 4;
                reg = reg >> 4;

                uint8_t val = byte[1];

                printf("%04x   SNE V%x, %d\n", pC, reg, val);

                break;
            }
        case 0x5:
            {
                uint8_t regX = byte[0] << 4;
                regX = regX >> 4;
                uint8_t regY = byte[1] >> 4;

                printf("%04x   SE V%x, V%x\n", pC, regX, regY);

                break;
            }
        case 0x6:
            {
                uint8_t val = byte[1];
                uint8_t reg = byte[0] << 4;
                reg = reg >> 4;

                printf("%04x   LD V%x, %d\n", pC, reg, val);

                break;
            }
        case 0x7:
            {
                uint8_t val = byte[1];
                uint8_t reg = byte[0] << 4;
                reg = reg >> 4;

                printf("%04x   ADD V%x, %d\n", pC, reg, val);

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
                        printf("%04x   LD V%x, V%x\n", pC, regX, regY);
                        break;
                    }
                    case 0x1:
                    {
                        printf("%04x   OR V%x, V%x\n", pC, regX, regY);
                        break;
                    }
                    case 0x2:
                    {
                        printf("%04x   AND V%x, V%x\n", pC, regX, regY);
                        break;
                    }
                    case 0x3:
                    {
                        printf("%04x   XOR V%x, V%x\n", pC, regX, regY);
                        break;
                    }
                    case 0x4:
                    {
                        printf("%04x   ADD V%x, V%x\n", pC, regX, regY);
                        break;
                    }
                    case 0x5:
                    {
                        printf("%04x   SUB V%x, V%x\n", pC, regX, regY);
                        break;
                    }
                    case 0x6:
                    {
                        printf("%04x   SHR V%x {, V%x}\n", pC, regX, regY);
                        break;
                    }
                    case 0x7:
                    {
                        printf("%04x    SUBN V%x, V%x\n", pC, regX, regY);
                        break;
                    }
                    case 0xe:
                    {
                        printf("%04x   SHL V%x {, V%x}\n", pC, regX, regY);
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

                printf("%04x   SNE V%x, V%x\n", pC, regX, regY);

                break;
            }
        case 0xa:
            {
                uint16_t address = byte[0] << 12;
                address = address >> 4;
                address = address | byte[1];

                printf("%04x   LD I, %x\n", pC, address);

                break;
            }
        case 0xb:
            {
                uint16_t address = byte[0] << 12;
                address = address >> 4;
                address = address | byte[1];

                printf("%04x   JP V0, %x\n", pC, address);

                break;
            }
        case 0xc:
            {
                uint8_t reg = byte[0] << 4;
                reg = reg >> 4;

                uint8_t val = byte[1];

                printf("%04x   RND V%x, %d\n", pC, reg, val);

                break;
            }
        case 0xd:
            {
                uint8_t regX = byte[0] << 4;
                regX = regX >> 4;

                uint8_t regY = byte[1] >> 4;
                uint8_t nibble = byte[1] << 4;
                nibble = nibble >> 4;

                printf("%04x   DRW V%x, V%x, %d\n", pC, regX, regY, nibble);

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
                        printf("%04x   SKP V%x\n", pC, reg);
                        break;
                    }
                    case 0xa1:
                    {
                        printf("%04x   SKNP V%x\n", pC, reg);
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
                        printf("%04x   LD V%x, DT\n", pC, reg);
                        break;
                    }
                    case 0x0a:
                    {
                        printf("%04x   LD V%x, K\n", pC, reg);
                        break;
                    }
                    case 0x15:
                    {
                        printf("%04x   LD DT, V%x\n", pC, reg);
                        break;
                    }
                    case 0x18:
                    {
                        printf("%04x   LD ST, V%x\n", pC, reg);
                        break;
                    }
                    case 0x1e:
                    {
                        printf("%04x   ADD I, V%x\n", pC, reg);
                        break;
                    }
                    case 0x29:
                    {
                        printf("%04x   LD F, V%x\n", pC, reg);
                        break;
                    }
                    case 0x33:
                    {
                        printf("%04x   LD B, V%x\n", pC, reg);
                        break;
                    }
                    case 0x55:
                    {
                        printf("%04x   LD [I], V%x\n", pC, reg);
                        break;
                    }
                    case 0x65:
                    {
                        printf("%04x   LD V%x, [I]\n", pC, reg);
                        break;
                    }
                }

                break;
            }
    }

    return;
}
