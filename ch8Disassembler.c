//Ryan Patrick

// This program takes chip8 assembly files as input and outputs
// to the assembly.out file.

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void decodeInstruction(char *machineCode, int pC, FILE *ofp);

int main()
{
    printf("Please enter the name of a chip8 rom: ");
    char fileName[256];
    scanf("%s", &fileName);

    FILE *rom = fopen(fileName, "rb");
    FILE *ofp = fopen("assembly.out", "w");


    // Get file size for memory allocation
    fseek(rom, 0, SEEK_END);
    long codeSize = ftell(rom);
    fseek(rom, 0, SEEK_SET);


    // Load the rom in at 0x200, 0x0-0x1FF is reserved for the interpreter
    char *machineCode = malloc(sizeof(0x200 + codeSize));
    fread(0x200 + machineCode, codeSize, 1, rom);
    fclose(rom);

    long memorySize = 0x200 + codeSize;
    int pC = 0x200; // Program Counter starts at 0x200


    fprintf(ofp, "ADDR | Instruction\n\n");


    // Each instruction is 2 bytes long, this loop loops through each instruction,
    // decodes it, and prints it to the output file
    while(pC < memorySize)
    {
        decodeInstruction(machineCode, pC, ofp);
        pC += 2;
    }


    printf("Disassembly Complete");
    fclose(ofp);

    // Free the memory allocated
    free(machineCode);

    return 0;
}


void decodeInstruction(char *machineCode, int pC, FILE *ofp)
{
    uint8_t *byte = &machineCode[pC];
    uint8_t halfByte = byte[0] >> 4; // Bitshift 4 bits right so we only look at first 4 bits of the instruction

    // This switch looks at the first 4 bits of the instruction
    // The first 4 bits help identify the instruction; in each case of the
    // switch, other bits of the instruction are examined to determine
    // which instruction is represented
    switch(halfByte)
    {
        case 0x0:
            {
                if(byte[0] == 0x0)
                {
                    if(byte[1] == 0xEE) // 00EE
                    {
                        fprintf(ofp, "%04x   RET\n", pC);
                    }
                    else if(byte[1] == 0xE0) // 00E0
                    {
                        fprintf(ofp, "%04x   CLS\n", pC);
                    }
                    else // 0nnn
                    {
                        uint16_t address = byte[0] << 12;
                        address = address >> 4;
                        address = address | byte[1];

                        fprintf(ofp, "%04x   SYS %x\n", pC, address);
                    }
                } // 0nnn
                else
                {
                    uint16_t address = byte[0] << 12;
                    address = address >> 4;
                    address = address | byte[1];

                    fprintf(ofp, "%04x   SYS %x\n", pC, address);
                }

                break;
            }
        case 0x1: // 1nnn
            {
                uint16_t address = byte[0] << 12;
                address = address >> 4;
                address = address | byte[1];

                fprintf(ofp, "%04x   JP %x\n", pC, address);

                break;
            }
        case 0x2: // 2nnn
            {
                uint16_t address = byte[0] << 12;
                address = address >> 4;
                address = address | byte[1];

                fprintf(ofp, "%04x   CALL %x\n", pC, address);

                break;
            }
        case 0x3: // 3xkk
            {
                uint8_t reg = byte[0] << 4;
                reg = reg >> 4;

                fprintf(ofp, "%04x   SE V%x, %02x\n", pC, reg, byte[1]);

                break;
            }
        case 0x4: // 4xkk
            {
                uint8_t reg = byte[0] << 4;
                reg = reg >> 4;

                fprintf(ofp, "%04x   SNE V%x, %02x\n", pC, reg, byte[1]);

                break;
            }
        case 0x5: // 5xy0
            {
                uint8_t nibble = byte[1] & 0xf;

                if(nibble == 0x0) // Check the last 4 bits to see if they are 0x0
                {
                    uint8_t regX = byte[0] << 4;
                    regX = regX >> 4;
                    uint8_t regY = byte[1] >> 4;

                    fprintf(ofp, "%04x   SE V%x, V%x\n", pC, regX, regY);
                }
                else // Not an instruction, print memory value
                {
                    fprintf(ofp, "%04x   %02x %02x\n", pC, byte[0], byte[1]);
                }

                break;
            }
        case 0x6: // 6xkk
            {
                uint8_t reg = byte[0] << 4;
                reg = reg >> 4;

                fprintf(ofp, "%04x   LD V%x, %02x\n", pC, reg, byte[1]);

                break;
            }
        case 0x7: //7xkk
            {
                uint8_t reg = byte[0] << 4;
                reg = reg >> 4;

                fprintf(ofp, "%04x   ADD V%x, %02x\n", pC, reg, byte[1]);

                break;
            }
        case 0x8:
            {
                uint8_t regX = byte[0] << 4;
                regX = regX >> 4;
                uint8_t regY = byte[1] >> 4;

                halfByte = byte[1] << 4;
                halfByte = halfByte >> 4;

                // This switch looks at the last 4 bits of the 2 byte instruction to
                // determine the instruction
                switch(halfByte)
                {
                    case 0x0: // 8xy0
                    {
                        fprintf(ofp, "%04x   LD V%x, V%x\n", pC, regX, regY);
                        break;
                    }
                    case 0x1: // 8xy1
                    {
                        fprintf(ofp, "%04x   OR V%x, V%x\n", pC, regX, regY);
                        break;
                    }
                    case 0x2: // 8xy2
                    {
                        fprintf(ofp, "%04x   AND V%x, V%x\n", pC, regX, regY);
                        break;
                    }
                    case 0x3: // 8xy3
                    {
                        fprintf(ofp, "%04x   XOR V%x, V%x\n", pC, regX, regY);
                        break;
                    }
                    case 0x4: // 8xy4
                    {
                        fprintf(ofp, "%04x   ADD V%x, V%x\n", pC, regX, regY);
                        break;
                    }
                    case 0x5: // 8xy5
                    {
                        fprintf(ofp, "%04x   SUB V%x, V%x\n", pC, regX, regY);
                        break;
                    }
                    case 0x6: // 8xy6
                    {
                        fprintf(ofp, "%04x   SHR V%x {, V%x}\n", pC, regX, regY);
                        break;
                    }
                    case 0x7: // 8xy7
                    {
                        fprintf(ofp, "%04x    SUBN V%x, V%x\n", pC, regX, regY);
                        break;
                    }
                    case 0xe: // 8xyE
                    {
                        fprintf(ofp, "%04x   SHL V%x {, V%x}\n", pC, regX, regY);
                        break;
                    }
                    // Instruction not matched, print memory value
                    default:
                    {
                        fprintf(ofp, "%04x   %02x %02x\n", pC, byte[0], byte[1]);
                        break;
                    }
                }

                break;
            }
        case 0x9: // 9xy0
            {
                uint8_t nibble = byte[1] & 0xf;

                if(nibble == 0x0) // Check the last 4 bits to see if they are 0x0
                {
                    uint8_t regX = byte[0] << 4;
                    regX = regX >> 4;

                    uint8_t regY = byte[1] >> 4;

                    fprintf(ofp, "%04x   SNE V%x, V%x\n", pC, regX, regY);
                }
                else // Not an instruction, print memory value
                {
                    fprintf(ofp, "%04x   %02x %02x\n", pC, byte[0], byte[1]);
                }
                break;
            }
        case 0xa: // Annn
            {
                uint16_t address = byte[0] << 12;
                address = address >> 4;
                address = address | byte[1];

                fprintf(ofp, "%04x   LD I, %x\n", pC, address);

                break;
            }
        case 0xb: // Bnnn
            {
                uint16_t address = byte[0] << 12;
                address = address >> 4;
                address = address | byte[1];

                fprintf(ofp, "%04x   JP V0, %x\n", pC, address);

                break;
            }
        case 0xc: // Cxkk
            {
                uint8_t reg = byte[0] << 4;
                reg = reg >> 4;

                fprintf(ofp, "%04x   RND V%x, %02x\n", pC, reg, byte[1]);

                break;
            }
        case 0xd: // Dxyn
            {
                uint8_t regX = byte[0] << 4;
                regX = regX >> 4;

                uint8_t regY = byte[1] >> 4;
                uint8_t nibble = byte[1] << 4;
                nibble = nibble >> 4;

                fprintf(ofp, "%04x   DRW V%x, V%x, %x\n", pC, regX, regY, nibble);

                break;
            }
        case 0xe:
            {
                uint8_t reg = byte[0] << 4;
                reg = reg >> 4;

                // This instruction is determined by the second byte
                switch(byte[1])
                {
                    case 0x9e: // Ex9E
                    {
                        fprintf(ofp, "%04x   SKP V%x\n", pC, reg);
                        break;
                    }
                    case 0xa1: // ExA1
                    {
                        fprintf(ofp, "%04x   SKNP V%x\n", pC, reg);
                        break;
                    }
                    // Instruction not matched, print memory value
                    default:
                    {
                        fprintf(ofp, "%04x   %02x %02x\n", pC, byte[0], byte[1]);
                        break;
                    }
                }

                break;
            }
        case 0xf:
            {
                uint8_t reg = byte[0] << 4;
                reg = reg >> 4;

                // This instruction is determined by the second byte
                switch(byte[1])
                {
                    case 0x07: // Fx07
                    {
                        fprintf(ofp, "%04x   LD V%x, DT\n", pC, reg);
                        break;
                    }
                    case 0x0a: // Fx0A
                    {
                        fprintf(ofp, "%04x   LD V%x, K\n", pC, reg);
                        break;
                    }
                    case 0x15: // Fx15
                    {
                        fprintf(ofp, "%04x   LD DT, V%x\n", pC, reg);
                        break;
                    }
                    case 0x18: // Fx18
                    {
                        fprintf(ofp, "%04x   LD ST, V%x\n", pC, reg);
                        break;
                    }
                    case 0x1e: // Fx1E
                    {
                        fprintf(ofp, "%04x   ADD I, V%x\n", pC, reg);
                        break;
                    }
                    case 0x29: // Fx29
                    {
                        fprintf(ofp, "%04x   LD F, V%x\n", pC, reg);
                        break;
                    }
                    case 0x33: // Fx33
                    {
                        fprintf(ofp, "%04x   LD B, V%x\n", pC, reg);
                        break;
                    }
                    case 0x55: // Fx55
                    {
                        fprintf(ofp, "%04x   LD [I], V%x\n", pC, reg);
                        break;
                    }
                    case 0x65: // Fx65
                    {
                        fprintf(ofp, "%04x   LD V%x, [I]\n", pC, reg);
                        break;
                    }
                    // Instruction not matched, print memory value
                    default:
                    {
                        fprintf(ofp, "%04x   %02x %02x\n", pC, byte[0], byte[1]);
                        break;
                    }
                }

                break;
            }
    }

    return;
}
