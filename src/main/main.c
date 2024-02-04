#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "cpu.h"

void loadROM(cpu* myCpu)
{
    cpu_write_8bit(myCpu, 0x8000, 0x78);
    cpu_write_8bit(myCpu, 0x8001, 0x9C);
    cpu_write_8bit(myCpu, 0x8002, 0x00);
    cpu_write_8bit(myCpu, 0x8003, 0x42);
    cpu_write_8bit(myCpu, 0x8004, 0x9C);
    cpu_write_8bit(myCpu, 0x8005, 0x0C);
    cpu_write_8bit(myCpu, 0x8006, 0x42);
    cpu_write_8bit(myCpu, 0x8007, 0x9C);
    cpu_write_8bit(myCpu, 0x8008, 0x0B);
    cpu_write_8bit(myCpu, 0x8009, 0x42);
    cpu_write_8bit(myCpu, 0x800A, 0x9C);
    cpu_write_8bit(myCpu, 0x800B, 0x40);
    cpu_write_8bit(myCpu, 0x800C, 0x21);
    cpu_write_8bit(myCpu, 0x800D, 0x9C);
    cpu_write_8bit(myCpu, 0x800E, 0x41);
    cpu_write_8bit(myCpu, 0x800F, 0x21);
    cpu_write_8bit(myCpu, 0x8010, 0x9C);
    cpu_write_8bit(myCpu, 0x8011, 0x42);
    cpu_write_8bit(myCpu, 0x8012, 0x21);
    cpu_write_8bit(myCpu, 0x8013, 0x9C);
    cpu_write_8bit(myCpu, 0x8014, 0x43);
    cpu_write_8bit(myCpu, 0x8015, 0x21);
}

int main()
{
    cpu* myCpu = malloc(sizeof(cpu));
    memset(myCpu, 0, sizeof(cpu));

    cpu_initialize(myCpu);

    loadROM(myCpu);

    cpu_reset(myCpu);

    cpu_write_16bit(myCpu, ADR_EMU_RESET, 0x8000);
    //cpu_write_8bit(myCpu, 0x8000, 0x4C);
    //cpu_write_8bit(myCpu, 0x8001, 0x34);
    //cpu_write_8bit(myCpu, 0x8002, 0x12);

    loadROM(myCpu);

    printf("RESET: %04x\n", cpu_read_16bit(myCpu, ADR_EMU_RESET));
    /*
    for(int i = 0x8000; i < 0x800F; i++)
    {
        printf("%04x: %02x\n", i, cpu_read_8bit(myCpu, i));
    }
    */

    cpu_reset(myCpu);
    
    for(int i = 0; i < 8; i++)
    {
        printf("$%06x: ", myCpu->current_op_code_pc);
        
        for(int j = 0; j < 6; j++)
        {
            if(j < myCpu->op_codes[myCpu->current_op_code].length)
                printf("%02x ", cpu_read_8bit(myCpu, myCpu->current_op_code_pc + j));
            else
                printf("   ");
        }

        printf("%s ", myCpu->op_codes[myCpu->current_op_code].instruction_name);
        for(int j = myCpu->op_codes[myCpu->current_op_code].length - 1; j >= 1; j--)
        {
            printf("%02x", cpu_read_8bit(myCpu, myCpu->current_op_code_pc + j));
        }

        printf("\n");
        cpu_clock(myCpu);
    }

    /*
    printf("====Instructions====\n");
    for(int i = 0; i <= 0xFF; i++)
        printf("%02x : %s \n", i, myCpu->op_codes[i].instruction_name);

    printf("====Accumulator====\nc: %i\na: %i\nb: %i\n\n",
    myCpu->accumulator_c,
    myCpu->accumulator_a,
    myCpu->accumulator_b);

    printf("====X Register====\nX: %i\nXL: %i\nXH: %i\n\n",
    myCpu->register_x,
    myCpu->register_x_l,
    myCpu->register_x_h);

    printf("====Y Register====\nY: %i\nYL: %i\nYH: %i\n\n",
    myCpu->register_y,
    myCpu->register_y_l,
    myCpu->register_y_h);

    //nvmxdizc
    printf("====Flags====\n");
    printf("n: %i\n",
    myCpu->flag_negative);
    printf("v: %i\n",
    myCpu->flag_overflow);
    printf("m: %i\n",
    myCpu->flag_accumulator_width);
    printf("x: %i\n",
    myCpu->flag_index_width);
    printf("b: %i\n",
    myCpu->flag_break);
    printf("d: %i\n",
    myCpu->flag_decimal_mode);
    printf("i: %i\n",
    myCpu->flag_interrupt_disable);
    printf("z: %i\n",
    myCpu->flag_zero);
    printf("c: %i\n",
    myCpu->flag_carry);

    printf("e: %i\n",
    myCpu->flag_emulation_mode);

    cpu_write_8bit(myCpu, 0x00, 0xFF);
    cpu_write_8bit(myCpu, 0x01, 0xAA);

    printf("16-bit 0x00: %04x\n", cpu_read_16bit(myCpu, 0x00));
    printf(" 8-bit 0x00: %02x\n", cpu_read_8bit(myCpu, 0x00));
    printf(" 8-bit 0x01: %02x\n", cpu_read_8bit(myCpu, 0x01));

    cpu_write_16bit(myCpu, 0x00, 0xAAFF);
    printf("16-bit 0x00: %04x\n", cpu_read_16bit(myCpu, 0x00));
    printf(" 8-bit 0x00: %02x\n", cpu_read_8bit(myCpu, 0x00));
    printf(" 8-bit 0x01: %02x\n", cpu_read_8bit(myCpu, 0x01));
    */

    free(myCpu);
    return 0;
}