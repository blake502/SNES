#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>

//The 65C816 uses these static address to determine where it should jump during some events,
//such as a non-maskable interrupt, or a reset.

//These are the destinations for those events outside of emulation mode:

static const u32 ADR_COP   = 0x00FFE4;
static const u32 ADR_BRK   = 0x00FFE6;
static const u32 ADR_ABORT = 0x00FFE8;
static const u32 ADR_NMI   = 0x00FFEA;
static const u32 ADR_IRQ   = 0x00FFEE;
//Note: There is no ADR_RESET because the flag_e is always set during a reset

//These are the destinations for those events during emulation mode:

static const u32 ADR_EMU_COP   = 0x00FFF4;
static const u32 ADR_EMU_BRK   = 0x00FFFE;
static const u32 ADR_EMU_ABORT = 0x00FFF8;
static const u32 ADR_EMU_NMI   = 0x00FFFA;
static const u32 ADR_EMU_RESET = 0x00FFFC;
static const u32 ADR_EMU_IRQ   = 0x00FFFE;

void cpu_initialize(cpu* myCpu)
{
    cpu_register_all_op_codes(myCpu);
    cpu_reset(myCpu);
}

void cpu_reset(cpu* myCpu)
{
    //I'm sure other things will need to be done here, but I'm not sure yet.

    //Reset the state of most things
    myCpu->accumulator_c = 0;
    myCpu->register_direct = 0;
    myCpu->register_processor_status = 0;
    myCpu->flag_emulation_mode = 1; //Emulation mode is always switched on during reset
    myCpu->flag_m = 1;
    myCpu->flag_x = 1;
    myCpu->stack_pointer = 0;
    myCpu->register_x = 0;
    myCpu->register_y = 0;
    myCpu->register_program_bank = 0;
    myCpu->register_data_bank = 0;

    //TODO: Reset RAM

    myCpu->current_instruction_full = 0;
    myCpu->current_op_code_pc = 0;

    myCpu->fetched_data = 0;
    myCpu->destination = 0;

    myCpu->cycles = 0;

    //The program counter begins at the 16bit address read from ADR_EMU_RESET (0x00FFFC)
    myCpu->program_counter = cpu_read_16bit(myCpu, ADR_EMU_RESET);
    
    cpu_clock(myCpu);
}

//Calculates the actual length of an instruction based on the length_mode
u8 cpu_get_instruction_length(cpu* myCpu, u8 base_length, u8 length_mode)
{
    switch (length_mode)
    {
    case 0:
        return base_length;
        break;
    case 1:
        return base_length - myCpu->flag_m;
        break;
    case 2:
        return base_length - myCpu->flag_x;
        break;
    default:
        break;
    }
    return base_length;
}

void cpu_clock(cpu* myCpu)
{
    //TODO: Remove "true" for accurate cycles
    //As it stands, each instruction will only take one cycle
    //If the CPU has completed its last instruction.
    if (myCpu->cycles == 0 || true)
    {
        //Clear out fetched data and instruction cache
        myCpu->destination = 0;
        myCpu->fetched_data = 0;
        myCpu->current_instruction_full = 0;
        
        //Set the current op code to the program counter
        myCpu->current_op_code_pc = myCpu->program_counter;
        //Read the next instruction
        myCpu->current_op_code = cpu_read_8bit(myCpu, myCpu->program_counter);

        //Load the op code for the instruction
        op_code op = myCpu->op_codes[myCpu->current_op_code];

        //Calculate the length of the instruction
        u8 instruction_length = cpu_get_instruction_length(myCpu, op.base_length, op.length_mode);

        //Read extra bytes if the instruction length is greater than one.
        if(instruction_length >= 2)
            myCpu->current_LL = cpu_read_8bit(myCpu, ++myCpu->program_counter);
        if(instruction_length >= 3)
            myCpu->current_HH = cpu_read_8bit(myCpu, ++myCpu->program_counter);
        if(instruction_length >= 4)
            myCpu->current_BB = cpu_read_8bit(myCpu, ++myCpu->program_counter);
        //Note-- The 65c816 opcodes guide refers to these bytes as LL, MM, and HH for some instructions
        //And considers them LL, HH, and unused for other instructions. I'm simplifying this by considering
        //them LL, HH, and BB in any case. For the few instructions that use MM-- use HH instead of MM; use BB instead of HH.

        //The addressing mode function should return 1 if a page boundary is crossed.
        u8 page_boundary_crossed = op.addressing_mode_func_ptr(myCpu);
        //The operation function should return 1 if a branch is taken
        u8 branch_taken = op.operation_func_ptr(myCpu);

        //Calculate how many cycles this operation should have taken
        //Even though the instruction complete immediately in the emulator, we must wait that many cycles before
        //Executing the next instruction to implement accurate cycles.
        myCpu->cycles = op.cycles_calculation_func_ptr(myCpu, op.base_cycles, page_boundary_crossed, branch_taken);

        //Each op code increments the program counter on its own.
        //Doing so here would break jumps and branches
        //myCpu->program_counter++;
    }
    //Decrement the amount of cycles until the next instruction can be executed
    myCpu->cycles--;
}

//This should probably be moved elsewhere.
//In the furture, this function should be called with a pointer to a byte buffer instead of a file name
void cpu_load_ROM(cpu* myCpu, const char* file_name)
{
    FILE *file;
    char *buffer;
    long file_size;

    //Open the file in binary mode
    fopen_s(&file, file_name, "rb");

    //Get the file size
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    rewind(file);

    //Allocate memory for the byte array
    buffer = (char*)malloc(file_size);

    //Read the file into RAM
    fread(myCpu->ram + 0x8000, 1, file_size, file);

    //Close the file and free the allocated memory
    fclose(file);
    free(buffer);

    cpu_reset(myCpu);
}

u16 cpu_read_16bit(cpu* myCpu, u32 address)
{
    u8 low = myCpu->ram[address];
    u8 high = myCpu->ram[address + 1];
    return low + (high << 8);
}

u8 cpu_read_8bit(cpu* myCpu, u32 address)
{
    return myCpu->ram[address];
}

void cpu_write_8bit(cpu* myCpu, u32 address, u8 data)
{
    myCpu->ram[address] = data;
}

void cpu_write_16bit(cpu* myCpu, u32 address, u16 data)
{
    u8 low = (u8)data;
    u8 high = data >> 8;
    myCpu->ram[address] = low;
    myCpu->ram[address + 1] = high;
}