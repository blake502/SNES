#include "cpu.h"

static const u32 ADR_COP   = 0x00FFE4;
static const u32 ADR_BRK   = 0x00FFE6;
static const u32 ADR_ABORT = 0x00FFE8;
static const u32 ADR_NMI   = 0x00FFEA;
static const u32 ADR_IRQ   = 0x00FFEE;

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
    myCpu->accumulator_c = 0;
    myCpu->register_direct = 0;
    myCpu->register_processor_status = 0;
    myCpu->flag_emulation_mode = 1;
    myCpu->stack_pointer = 0;
    myCpu->register_x = 0;
    myCpu->register_y = 0;
    myCpu->register_program_bank = 0;
    myCpu->register_data_bank = 0;

    //TODO: Reset RAM

    myCpu->current_op_code = 0;
    myCpu->current_op_code_pc = 0;

    myCpu->fetched_data = 0;
    myCpu->destination = 0;

    myCpu->cycles = 0;

    myCpu->program_counter = cpu_read_16bit(myCpu, ADR_EMU_RESET);
    
    cpu_clock(myCpu);
}

void cpu_clock(cpu* myCpu)
{
    //TODO: Remove "true" for accurate cycles
    //As it stands, each instruction will only take one cycle
    if (myCpu->cycles == 0 || true)
    {
        myCpu->destination = 0;
        myCpu->fetched_data = 0;
        
        u8 next_instruction = cpu_read_8bit(myCpu, myCpu->program_counter);
        myCpu->current_op_code_pc = myCpu->program_counter;
        myCpu->current_op_code = next_instruction;

        op_code op = myCpu->op_codes[next_instruction];


        u8 page_boundary_crossed = op.addressing_mode(myCpu);
        u8 branch_taken = op.operation_function_pointer(myCpu);

        myCpu->cycles = op.operation_cycles(myCpu, op.cycles, page_boundary_crossed, branch_taken);
        myCpu->program_counter++;
    }
    myCpu->cycles--;
}

void cpu_load_ROM(cpu* myCpu)
{
    //TODO: Implement ROM loading.
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