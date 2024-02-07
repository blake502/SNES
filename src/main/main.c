#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "cpu.h"

//TODO: Remove forward declaration of debugging functions
void print_instruction(cpu* myCpu);
void print_flags(cpu* myCpu);

int main()
{
    cpu* myCpu = malloc(sizeof(cpu));
    memset(myCpu, 0, sizeof(cpu));

    cpu_initialize(myCpu);

    cpu_load_ROM(myCpu, "smw.smc");

    cpu_reset(myCpu);

    //printf("TEST: %02x\n", 0b1111&(~0b01));

    /*
    printf("Flags:\n");
    print_flags(myCpu);
    printf("Setting C and N and E flags\n");
    cpu_flags_set(myCpu, flag_mask_c | flag_mask_n | flag_mask_e);
    printf("Flags:\n");
    print_flags(myCpu);
    printf("Resetting C and N and E flags\n");
    cpu_flags_reset(myCpu, flag_mask_c | flag_mask_n | flag_mask_e);
    printf("Flags:\n");
    print_flags(myCpu);
    */

    //20 clock cycles
    for(int i = 0; i < 20; i++)
    {
        print_instruction(myCpu);
        cpu_clock(myCpu);
    }

    free(myCpu);

    

    return 0;
}

//TODO: Remove this temporary debugger/helper functions
void print_flags(cpu* myCpu)
{
    printf("nvmxdizc e\n%c%c%c%c%c%c%c%c %c\n",
        myCpu->flag_n ? '1' : '0',
        myCpu->flag_v ? '1' : '0',
        myCpu->flag_m ? '1' : '0',
        myCpu->flag_x ? '1' : '0',
        myCpu->flag_d ? '1' : '0',
        myCpu->flag_i ? '1' : '0',
        myCpu->flag_z ? '1' : '0',
        myCpu->flag_c ? '1' : '0',
        myCpu->flag_e ? '1' : '0'
    );
}

void print_instruction(cpu* myCpu)
{
    printf("$%06x : %02x ",
            myCpu->current_op_code_pc,
            myCpu->current_op_code
        );

        op_code op = myCpu->op_codes[myCpu->current_op_code];
        u8 instruction_length = cpu_get_instruction_length(myCpu, op.base_length, op.length_mode);

        if(instruction_length > 1)
            printf("%02x ", myCpu->current_LL);
        else
            printf("   ");

        if(instruction_length > 2)
            printf("%02x ", myCpu->current_HH);
        else
            printf("   ");

        if(instruction_length > 3)
            printf("%02x ", myCpu->current_BB);
        else
            printf("   ");

        printf("%s ", op.instruction_name);

        if(instruction_length > 3)
            printf("%02x", myCpu->current_BB);

        if(instruction_length > 2)
            printf("%02x", myCpu->current_HH);

        if(instruction_length > 1)
            printf("%02x", myCpu->current_LL);

        printf("\n");
}
