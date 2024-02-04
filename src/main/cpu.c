#include "cpu.h"

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

#pragma region Cycles count modes
//Absolute
u8 cycle_count_mode00(cpu* myCpu, u8 absolute, u8 page_boundary_crossed, u8 branch_taken)
{
    return absolute;
}

//Absolute+e*p
u8 cycle_count_mode01(cpu* myCpu, u8 absolute, u8 page_boundary_crossed, u8 branch_taken)
{
    return absolute+myCpu->flag_e*page_boundary_crossed;
}

//Absolute+t+t*e*p
u8 cycle_count_mode02(cpu* myCpu, u8 absolute, u8 page_boundary_crossed, u8 branch_taken)
{
    return absolute+branch_taken+branch_taken*myCpu->flag_e*page_boundary_crossed;
}

//Absolute+w
u8 cycle_count_mode03(cpu* myCpu, u8 absolute, u8 page_boundary_crossed, u8 branch_taken)
{
    u8 w = myCpu->register_dl == 0 ? 0 : 1;
    return absolute+w;
}

//Absolute-2*m
u8 cycle_count_mode04(cpu* myCpu, u8 absolute, u8 page_boundary_crossed, u8 branch_taken)
{
    return absolute-2*myCpu->flag_m;
}

//Absolute-2*m+w
u8 cycle_count_mode05(cpu* myCpu, u8 absolute, u8 page_boundary_crossed, u8 branch_taken)
{
    u8 w = myCpu->register_dl == 0 ? 0 : 1;
    return absolute-2*myCpu->flag_m+w;
}

//Absolute-2*x+x*p
u8 cycle_count_mode06(cpu* myCpu, u8 absolute, u8 page_boundary_crossed, u8 branch_taken)
{
    return absolute-2*myCpu->flag_x+myCpu->flag_x*page_boundary_crossed;
}

//Absolute-e
u8 cycle_count_mode07(cpu* myCpu, u8 absolute, u8 page_boundary_crossed, u8 branch_taken)
{
    return absolute-myCpu->flag_e;
}

//Absolute-m
u8 cycle_count_mode08(cpu* myCpu, u8 absolute, u8 page_boundary_crossed, u8 branch_taken)
{
    return absolute-myCpu->flag_m;
}

//Absolute-m+w
u8 cycle_count_mode09(cpu* myCpu, u8 absolute, u8 page_boundary_crossed, u8 branch_taken)
{
    u8 w = myCpu->register_dl == 0 ? 0 : 1;
    return absolute-myCpu->flag_m+w;
}

//Absolute-m+w-x+x*p
u8 cycle_count_mode0A(cpu* myCpu, u8 absolute, u8 page_boundary_crossed, u8 branch_taken)
{
    u8 w = myCpu->register_dl == 0 ? 0 : 1;
    return absolute-myCpu->flag_m+w-myCpu->flag_x+myCpu->flag_x*page_boundary_crossed;
}

//Absolute-m-x+x*p
u8 cycle_count_mode0B(cpu* myCpu, u8 absolute, u8 page_boundary_crossed, u8 branch_taken)
{
    return absolute-myCpu->flag_m-myCpu->flag_x+myCpu->flag_x*page_boundary_crossed;
}

//Absolute-x
u8 cycle_count_mode0C(cpu* myCpu, u8 absolute, u8 page_boundary_crossed, u8 branch_taken)
{
    return absolute-myCpu->flag_x;
}

//Absolute-x+w
u8 cycle_count_mode0D(cpu* myCpu, u8 absolute, u8 page_boundary_crossed, u8 branch_taken)
{
    u8 w = myCpu->register_dl == 0 ? 0 : 1;
    return absolute-myCpu->flag_x+w;
}
#pragma endregion

#pragma region Adressing modes

//ABSOLUTE
//Length: 3 bytes, $OP $LL $HH 
u8 addressing_mode_abs(cpu* myCpu)
{
    myCpu->program_counter++;
    u8 LL = cpu_read_8bit(myCpu, myCpu->program_counter);

    myCpu->program_counter++;
    u8 HH = cpu_read_8bit(myCpu, myCpu->program_counter);

    myCpu->destination_bank = myCpu->register_program_bank;

    myCpu->destination_lo = LL;
    
    myCpu->destination_hi = HH;
    

    u32 data_address =
        (myCpu->register_data_bank << 16) +
        (HH << 8) +
        LL;
    
    myCpu->fetched_data_lo = cpu_read_8bit(myCpu, data_address);
    myCpu->fetched_data_hi = cpu_read_8bit(myCpu, data_address + 1);
    
    return 0;
}

//ABSOLUTE,X
//Length: 3 bytes, $OP $LL $HH
u8 addressing_mode_abs_offset_x(cpu* myCpu)
{
    myCpu->program_counter++;
    u8 LL = cpu_read_8bit(myCpu, myCpu->program_counter);

    myCpu->program_counter++;
    u8 HH = cpu_read_8bit(myCpu, myCpu->program_counter);

    u32 data_address =
        (myCpu->register_data_bank << 16) +
        (HH << 8) +
        LL + myCpu->register_x;

    myCpu->fetched_data_lo = cpu_read_8bit(myCpu, data_address);
    myCpu->fetched_data_hi = cpu_read_8bit(myCpu, data_address + 1);

    return 0;
}

//ABSOLUTE,Y
//Length: 3 bytes, $OP $LL $HH
u8 addressing_mode_abs_offset_y(cpu* myCpu)
{
    myCpu->program_counter++;
    u8 LL = cpu_read_8bit(myCpu, myCpu->program_counter);

    myCpu->program_counter++;
    u8 HH = cpu_read_8bit(myCpu, myCpu->program_counter);

    u32 data_address =
        (myCpu->register_data_bank << 16) +
        (HH << 8) +
        LL + myCpu->register_y;

    myCpu->fetched_data_lo = cpu_read_8bit(myCpu, data_address);
    myCpu->fetched_data_hi = cpu_read_8bit(myCpu, data_address + 1);

    return 0;
}

//(ABSOLUTE)
//Length: 3 bytes, $OP $LL $HH 
//(absolute) addressing uses a 16-bit pointer.
u8 addressing_mode_abs_16bit(cpu* myCpu)
{
    myCpu->program_counter++;
    u8 LL = cpu_read_8bit(myCpu, myCpu->program_counter);

    myCpu->program_counter++;
    u8 HH = cpu_read_8bit(myCpu, myCpu->program_counter);

    u32 data_address =
        (0 << 16) +
        (HH << 8) +
        LL + myCpu->register_y;

    myCpu->fetched_data_lo = cpu_read_8bit(myCpu, data_address);
    myCpu->fetched_data_hi = cpu_read_8bit(myCpu, data_address + 1);

    myCpu->destination_lo = myCpu->fetched_data_lo;
    myCpu->destination_hi = myCpu->fetched_data_hi;
    myCpu->destination_bank = myCpu->register_program_bank;

    return 0;
}

//[ABSOLUTE]
//Length: 3 bytes, $OP $LL $HH 
//[absolute] addressing uses a 24-bit pointer.
u8 addressing_mode_abs_24bit(cpu* myCpu)
{
    myCpu->program_counter++;
    u8 LL = cpu_read_8bit(myCpu, myCpu->program_counter);

    myCpu->program_counter++;
    u8 HH = cpu_read_8bit(myCpu, myCpu->program_counter);

    u32 data_address =
        (0 << 16) +
        (HH << 8) +
        LL + myCpu->register_y;

    myCpu->fetched_data_lo = cpu_read_8bit(myCpu, data_address);
    myCpu->fetched_data_hi = cpu_read_8bit(myCpu, data_address + 1);
    u8 fetched_data_bank = cpu_read_8bit(myCpu, data_address + 2);

    myCpu->destination_lo = myCpu->fetched_data_lo;
    myCpu->destination_hi = myCpu->fetched_data_hi;
    myCpu->destination_bank = fetched_data_bank;

    return 0;
}

//(ABSOLUTE,X)
//Length: 3 bytes, $OP $LL $HH 
//(absolute,X) addressing uses a 16-bit pointer
u8 addressing_mode_abs_offset_x_bank(cpu* myCpu)
{
    myCpu->program_counter++;
    u8 LL = cpu_read_8bit(myCpu, myCpu->program_counter);

    myCpu->program_counter++;
    u8 HH = cpu_read_8bit(myCpu, myCpu->program_counter);

    u32 data_address =
        (0 << 16) +
        (HH << 8) +
        LL + myCpu->register_x;

    myCpu->fetched_data_lo = cpu_read_8bit(myCpu, data_address);
    myCpu->fetched_data_hi = cpu_read_8bit(myCpu, data_address + 1);

    myCpu->destination_lo = myCpu->fetched_data_lo;
    myCpu->destination_hi = myCpu->fetched_data_hi;
    myCpu->destination_bank = myCpu->register_program_bank;

    return 0;
}

//ACCUMULATOR
//Length: 1 byte, $OP 
u8 addressing_mode_acc(cpu* myCpu)
{
    myCpu->fetched_data = myCpu->accumulator_c;
    return 0;
}

//DIRECT
//Length: 2 bytes, $OP $LL 
u8 addressing_mode_dir(cpu* myCpu)
{
    u8 OP = cpu_read_8bit(myCpu, myCpu->program_counter);
    myCpu->program_counter++;
    u8 LL = cpu_read_8bit(myCpu, myCpu->program_counter);

    if(OP != 0xD4 && myCpu->flag_e == 1 && myCpu->register_dl == 0)
    {
        u32 data_address =
            (0 << 16) +
            (myCpu->register_dh << 8) +
            LL;

        myCpu->fetched_data_lo = cpu_read_8bit(myCpu, data_address);
    }
    else
    {
        u32 data_address =
            (0 << 16) +
            (myCpu->register_direct) +
            LL;

        myCpu->fetched_data_lo = cpu_read_8bit(myCpu, data_address);
        myCpu->fetched_data_hi = cpu_read_8bit(myCpu, data_address + 1);
    }

    return 0;
}

// DIRECT,X
//Length: 2 bytes, $OP $LL 
u8 addressing_mode_dir_offset_x(cpu* myCpu)
{
    myCpu->program_counter++;
    u8 LL = cpu_read_8bit(myCpu, myCpu->program_counter);

    if(myCpu->flag_e == 1 && myCpu->register_dl == 0)
    {
        u32 data_address =
            (0 << 16) +
            (myCpu->register_dh << 8) +
            LL + myCpu->register_x;

        myCpu->fetched_data_lo = cpu_read_8bit(myCpu, data_address);
    }
    else
    {
        u32 data_address =
            (0 << 16) +
            (myCpu->register_direct) +
            LL + myCpu->register_x;

        myCpu->fetched_data_lo = cpu_read_8bit(myCpu, data_address);
        myCpu->fetched_data_hi = cpu_read_8bit(myCpu, data_address + 1);
    }

    return 0;
}

// DIRECT,Y
//Length: 2 bytes, $OP $LL 
u8 addressing_mode_dir_offset_y(cpu* myCpu)
{
    myCpu->program_counter++;
    u8 LL = cpu_read_8bit(myCpu, myCpu->program_counter);

    if(myCpu->flag_e == 1 && myCpu->register_dl == 0)
    {
        u32 data_address =
            (0 << 16) +
            (myCpu->register_dh << 8) +
            LL + myCpu->register_y;

        myCpu->fetched_data_lo = cpu_read_8bit(myCpu, data_address);
    }
    else
    {
        u32 data_address =
            (0 << 16) +
            (myCpu->register_direct) +
            LL + myCpu->register_y;

        myCpu->fetched_data_lo = cpu_read_8bit(myCpu, data_address);
        myCpu->fetched_data_hi = cpu_read_8bit(myCpu, data_address + 1);
    }

    return 0;
}

u8 addressing_mode_dir_16bit(cpu* myCpu)
{
    return 0;
}

u8 addressing_mode_dir_24bit(cpu* myCpu)
{
    return 0;
}

u8 addressing_mode_dir_offset_x_bank(cpu* myCpu)
{
    return 0;
}

u8 addressing_mode_dir_y_16bit(cpu* myCpu)
{
    return 0;
}

u8 addressing_mode_dir_y_24bit(cpu* myCpu)
{
    return 0;
}

u8 addressing_mode_imm(cpu* myCpu)
{
    return 0;
}

u8 addressing_mode_imp(cpu* myCpu)
{
    return 0;
}

u8 addressing_mode_long(cpu* myCpu)
{
    return 0;
}

u8 addressing_mode_long_x(cpu* myCpu)
{
    return 0;
}

u8 addressing_mode_rel_8(cpu* myCpu)
{
    return 0;
}

u8 addressing_mode_rel_16(cpu* myCpu)
{
    return 0;
}

u8 addressing_mode_src_dest(cpu* myCpu)
{
    return 0;
}

u8 addressing_mode_stk_s(cpu* myCpu)
{
    return 0;
}

u8 addressing_mode_stk_s_y(cpu* myCpu)
{
    return 0;
}

#pragma endregion

#pragma region operation functions
u8 operation_function_ADC(cpu* myCpu)
{
    return 0;
}

u8 operation_function_AND(cpu* myCpu)
{
    return 0;
}

u8 operation_function_ASL(cpu* myCpu)
{
    return 0;
}

u8 operation_function_BCC(cpu* myCpu)
{
    return 0;
}

u8 operation_function_BCS(cpu* myCpu)
{
    return 0;
}

u8 operation_function_BEQ(cpu* myCpu)
{
    return 0;
}

u8 operation_function_BIT(cpu* myCpu)
{
    return 0;
}

u8 operation_function_BMI(cpu* myCpu)
{
    return 0;
}

u8 operation_function_BNE(cpu* myCpu)
{
    return 0;
}

u8 operation_function_BPL(cpu* myCpu)
{
    return 0;
}

u8 operation_function_BRA(cpu* myCpu)
{
    return 0;
}

u8 operation_function_BRK(cpu* myCpu)
{
    return 0;
}

u8 operation_function_BRL(cpu* myCpu)
{
    return 0;
}

u8 operation_function_BVC(cpu* myCpu)
{
    return 0;
}

u8 operation_function_BVS(cpu* myCpu)
{
    return 0;
}

u8 operation_function_CLC(cpu* myCpu)
{
    return 0;
}

u8 operation_function_CLD(cpu* myCpu)
{
    return 0;
}

u8 operation_function_CLI(cpu* myCpu)
{
    return 0;
}

u8 operation_function_CLV(cpu* myCpu)
{
    return 0;
}

u8 operation_function_CMP(cpu* myCpu)
{
    return 0;
}

u8 operation_function_COP(cpu* myCpu)
{
    return 0;
}

u8 operation_function_CPX(cpu* myCpu)
{
    return 0;
}

u8 operation_function_CPY(cpu* myCpu)
{
    return 0;
}

u8 operation_function_DEC(cpu* myCpu)
{
    return 0;
}

u8 operation_function_DEX(cpu* myCpu)
{
    return 0;
}

u8 operation_function_DEY(cpu* myCpu)
{
    return 0;
}

u8 operation_function_EOR(cpu* myCpu)
{
    return 0;
}

u8 operation_function_INC(cpu* myCpu)
{
    return 0;
}

u8 operation_function_INX(cpu* myCpu)
{
    return 0;
}

u8 operation_function_INY(cpu* myCpu)
{
    return 0;
}

u8 operation_function_JMP(cpu* myCpu)
{
    myCpu->program_counter = myCpu->destination;
    myCpu->register_program_bank = myCpu->destination_bank;
    return 0;
}

u8 operation_function_JSL(cpu* myCpu)
{
    return 0;
}

u8 operation_function_JSR(cpu* myCpu)
{
    return 0;
}

u8 operation_function_LDA(cpu* myCpu)
{
    return 0;
}

u8 operation_function_LDX(cpu* myCpu)
{
    return 0;
}

u8 operation_function_LDY(cpu* myCpu)
{
    return 0;
}

u8 operation_function_LSR(cpu* myCpu)
{
    return 0;
}

u8 operation_function_MVN(cpu* myCpu)
{
    return 0;
}

u8 operation_function_MVP(cpu* myCpu)
{
    return 0;
}

u8 operation_function_NOP(cpu* myCpu)
{
    return 0;
}

u8 operation_function_ORA(cpu* myCpu)
{
    return 0;
}

u8 operation_function_PEA(cpu* myCpu)
{
    return 0;
}

u8 operation_function_PEI(cpu* myCpu)
{
    return 0;
}

u8 operation_function_PER(cpu* myCpu)
{
    return 0;
}

u8 operation_function_PHA(cpu* myCpu)
{
    return 0;
}

u8 operation_function_PHB(cpu* myCpu)
{
    return 0;
}

u8 operation_function_PHD(cpu* myCpu)
{
    return 0;
}

u8 operation_function_PHK(cpu* myCpu)
{
    return 0;
}

u8 operation_function_PHP(cpu* myCpu)
{
    return 0;
}

u8 operation_function_PHX(cpu* myCpu)
{
    return 0;
}

u8 operation_function_PHY(cpu* myCpu)
{
    return 0;
}

u8 operation_function_PLA(cpu* myCpu)
{
    return 0;
}

u8 operation_function_PLB(cpu* myCpu)
{
    return 0;
}

u8 operation_function_PLD(cpu* myCpu)
{
    return 0;
}

u8 operation_function_PLP(cpu* myCpu)
{
    return 0;
}

u8 operation_function_PLX(cpu* myCpu)
{
    return 0;
}

u8 operation_function_PLY(cpu* myCpu)
{
    return 0;
}

u8 operation_function_REP(cpu* myCpu)
{
    return 0;
}

u8 operation_function_ROL(cpu* myCpu)
{
    return 0;
}

u8 operation_function_ROR(cpu* myCpu)
{
    return 0;
}

u8 operation_function_RTI(cpu* myCpu)
{
    return 0;
}

u8 operation_function_RTL(cpu* myCpu)
{
    return 0;
}

u8 operation_function_RTS(cpu* myCpu)
{
    return 0;
}

u8 operation_function_SBC(cpu* myCpu)
{
    return 0;
}

u8 operation_function_SEC(cpu* myCpu)
{
    return 0;
}

u8 operation_function_SED(cpu* myCpu)
{
    return 0;
}

u8 operation_function_SEI(cpu* myCpu)
{
    return 0;
}

u8 operation_function_SEP(cpu* myCpu)
{
    return 0;
}

u8 operation_function_STA(cpu* myCpu)
{
    return 0;
}

u8 operation_function_STP(cpu* myCpu)
{
    return 0;
}

u8 operation_function_STX(cpu* myCpu)
{
    return 0;
}

u8 operation_function_STY(cpu* myCpu)
{
    return 0;
}

u8 operation_function_STZ(cpu* myCpu)
{
    return 0;
}

u8 operation_function_TAX(cpu* myCpu)
{
    return 0;
}

u8 operation_function_TAY(cpu* myCpu)
{
    return 0;
}

u8 operation_function_TCD(cpu* myCpu)
{
    return 0;
}

u8 operation_function_TCS(cpu* myCpu)
{
    return 0;
}

u8 operation_function_TDC(cpu* myCpu)
{
    return 0;
}

u8 operation_function_TRB(cpu* myCpu)
{
    return 0;
}

u8 operation_function_TSB(cpu* myCpu)
{
    return 0;
}

u8 operation_function_TSC(cpu* myCpu)
{
    return 0;
}

u8 operation_function_TSX(cpu* myCpu)
{
    return 0;
}

u8 operation_function_TXA(cpu* myCpu)
{
    return 0;
}

u8 operation_function_TXS(cpu* myCpu)
{
    return 0;
}

u8 operation_function_TXY(cpu* myCpu)
{
    return 0;
}

u8 operation_function_TYA(cpu* myCpu)
{
    return 0;
}

u8 operation_function_TYX(cpu* myCpu)
{
    return 0;
}

u8 operation_function_WAI(cpu* myCpu)
{
    return 0;
}

u8 operation_function_WDM(cpu* myCpu)
{
    return 0;
}

u8 operation_function_XBA(cpu* myCpu)
{
    return 0;
}

u8 operation_function_XCE(cpu* myCpu)
{
    return 0;
}

#pragma endregion

void cpu_add_op_code(
    cpu* myCpu,
    const char* name,
    u8 (*addressing_mode)(cpu* myCpu),
    u8 (*operation_function_pointer)(cpu* myCpu),
    u8 (*operation_cycles)(cpu* myCpu, u8 absolute, u8 page_boundary_crossed, u8 branch_taken),
    u8 cycles,
    u16 opcode,
    u8 length,
    u8 length_mode
    )
{
    op_code myOp_code;
    myOp_code.instruction_name = name;
    myOp_code.addressing_mode = addressing_mode;
    myOp_code.operation_function_pointer = operation_function_pointer;
    myOp_code.operation_cycles = operation_cycles;
    myOp_code.cycles = cycles;
    myOp_code.length = length;
    myOp_code.length_mode = length_mode;
    myCpu->op_codes[opcode] = myOp_code;
}

void cpu_initialize(cpu* myCpu)
{
    cpu_add_op_code(myCpu, "BRK", addressing_mode_imp, operation_function_BRK, cycle_count_mode07, 8, 0x0, 1, 0);
    cpu_add_op_code(myCpu, "ORA", addressing_mode_dir_offset_x_bank, operation_function_ORA, cycle_count_mode09, 7, 0x1, 2, 0);
    cpu_add_op_code(myCpu, "COP", addressing_mode_imm, operation_function_COP, cycle_count_mode07, 8, 0x2, 2, 0);
    cpu_add_op_code(myCpu, "ORA", addressing_mode_stk_s, operation_function_ORA, cycle_count_mode08, 5, 0x3, 2, 0);
    cpu_add_op_code(myCpu, "TSB", addressing_mode_dir, operation_function_TSB, cycle_count_mode05, 7, 0x4, 2, 0);
    cpu_add_op_code(myCpu, "ORA", addressing_mode_dir, operation_function_ORA, cycle_count_mode09, 4, 0x5, 2, 0);
    cpu_add_op_code(myCpu, "ASL", addressing_mode_dir, operation_function_ASL, cycle_count_mode05, 7, 0x6, 2, 0);
    cpu_add_op_code(myCpu, "ORA", addressing_mode_dir_24bit, operation_function_ORA, cycle_count_mode09, 7, 0x7, 2, 0);
    cpu_add_op_code(myCpu, "PHP", addressing_mode_imp, operation_function_PHP, cycle_count_mode00, 3, 0x8, 1, 0);
    cpu_add_op_code(myCpu, "ORA", addressing_mode_imm, operation_function_ORA, cycle_count_mode08, 3, 0x9, 3, 1);
    cpu_add_op_code(myCpu, "ASL", addressing_mode_acc, operation_function_ASL, cycle_count_mode00, 2, 0x0A, 1, 0);
    cpu_add_op_code(myCpu, "PHD", addressing_mode_imp, operation_function_PHD, cycle_count_mode00, 4, 0x0B, 1, 0);
    cpu_add_op_code(myCpu, "TSB", addressing_mode_abs, operation_function_TSB, cycle_count_mode04, 8, 0x0C, 3, 0);
    cpu_add_op_code(myCpu, "ORA", addressing_mode_abs, operation_function_ORA, cycle_count_mode08, 5, 0x0D, 3, 0);
    cpu_add_op_code(myCpu, "ASL", addressing_mode_abs, operation_function_ASL, cycle_count_mode04, 8, 0x0E, 3, 0);
    cpu_add_op_code(myCpu, "ORA", addressing_mode_long, operation_function_ORA, cycle_count_mode08, 6, 0x0F, 4, 0);
    cpu_add_op_code(myCpu, "BPL", addressing_mode_rel_8, operation_function_BPL, cycle_count_mode02, 2, 0x10, 2, 0);
    cpu_add_op_code(myCpu, "ORA", addressing_mode_dir_y_16bit, operation_function_ORA, cycle_count_mode0A, 7, 0x11, 2, 0);
    cpu_add_op_code(myCpu, "ORA", addressing_mode_dir_16bit, operation_function_ORA, cycle_count_mode09, 6, 0x12, 2, 0);
    cpu_add_op_code(myCpu, "ORA", addressing_mode_stk_s_y, operation_function_ORA, cycle_count_mode08, 8, 0x13, 2, 0);
    cpu_add_op_code(myCpu, "TRB", addressing_mode_dir, operation_function_TRB, cycle_count_mode05, 7, 0x14, 2, 0);
    cpu_add_op_code(myCpu, "ORA", addressing_mode_dir_offset_x, operation_function_ORA, cycle_count_mode09, 5, 0x15, 2, 0);
    cpu_add_op_code(myCpu, "ASL", addressing_mode_dir_offset_x, operation_function_ASL, cycle_count_mode05, 8, 0x16, 2, 0);
    cpu_add_op_code(myCpu, "ORA", addressing_mode_dir_y_24bit, operation_function_ORA, cycle_count_mode09, 7, 0x17, 2, 0);
    cpu_add_op_code(myCpu, "CLC", addressing_mode_imp, operation_function_CLC, cycle_count_mode00, 2, 0x18, 1, 0);
    cpu_add_op_code(myCpu, "ORA", addressing_mode_abs_offset_y, operation_function_ORA, cycle_count_mode0B, 6, 0x19, 3, 0);
    cpu_add_op_code(myCpu, "INC", addressing_mode_acc, operation_function_INC, cycle_count_mode00, 2, 0x1A, 1, 0);
    cpu_add_op_code(myCpu, "TCS", addressing_mode_imp, operation_function_TCS, cycle_count_mode00, 2, 0x1B, 1, 0);
    cpu_add_op_code(myCpu, "TRB", addressing_mode_abs, operation_function_TRB, cycle_count_mode04, 8, 0x1C, 3, 0);
    cpu_add_op_code(myCpu, "ORA", addressing_mode_abs_offset_x, operation_function_ORA, cycle_count_mode0B, 6, 0x1D, 3, 0);
    cpu_add_op_code(myCpu, "ASL", addressing_mode_abs_offset_x, operation_function_ASL, cycle_count_mode04, 9, 0x1E, 3, 0);
    cpu_add_op_code(myCpu, "ORA", addressing_mode_long_x, operation_function_ORA, cycle_count_mode08, 6, 0x1F, 4, 0);
    cpu_add_op_code(myCpu, "JSR", addressing_mode_abs, operation_function_JSR, cycle_count_mode00, 6, 0x20, 3, 0);
    cpu_add_op_code(myCpu, "AND", addressing_mode_dir_offset_x_bank, operation_function_AND, cycle_count_mode09, 7, 0x21, 2, 0);
    cpu_add_op_code(myCpu, "JSL", addressing_mode_long, operation_function_JSL, cycle_count_mode00, 8, 0x22, 4, 0);
    cpu_add_op_code(myCpu, "AND", addressing_mode_stk_s, operation_function_AND, cycle_count_mode08, 5, 0x23, 2, 0);
    cpu_add_op_code(myCpu, "BIT", addressing_mode_dir, operation_function_BIT, cycle_count_mode09, 4, 0x24, 2, 0);
    cpu_add_op_code(myCpu, "AND", addressing_mode_dir, operation_function_AND, cycle_count_mode09, 4, 0x25, 2, 0);
    cpu_add_op_code(myCpu, "ROL", addressing_mode_dir, operation_function_ROL, cycle_count_mode05, 7, 0x26, 2, 0);
    cpu_add_op_code(myCpu, "AND", addressing_mode_dir_24bit, operation_function_AND, cycle_count_mode09, 7, 0x27, 2, 0);
    cpu_add_op_code(myCpu, "PLP", addressing_mode_imp, operation_function_PLP, cycle_count_mode00, 4, 0x28, 1, 0);
    cpu_add_op_code(myCpu, "AND", addressing_mode_imm, operation_function_AND, cycle_count_mode08, 3, 0x29, 3, 1);
    cpu_add_op_code(myCpu, "ROL", addressing_mode_acc, operation_function_ROL, cycle_count_mode00, 2, 0x2A, 1, 0);
    cpu_add_op_code(myCpu, "PLD", addressing_mode_imp, operation_function_PLD, cycle_count_mode00, 5, 0x2B, 1, 0);
    cpu_add_op_code(myCpu, "BIT", addressing_mode_abs, operation_function_BIT, cycle_count_mode08, 5, 0x2C, 3, 0);
    cpu_add_op_code(myCpu, "AND", addressing_mode_abs, operation_function_AND, cycle_count_mode08, 5, 0x2D, 3, 0);
    cpu_add_op_code(myCpu, "ROL", addressing_mode_abs, operation_function_ROL, cycle_count_mode04, 8, 0x2E, 3, 0);
    cpu_add_op_code(myCpu, "AND", addressing_mode_long, operation_function_AND, cycle_count_mode08, 6, 0x2F, 4, 0);
    cpu_add_op_code(myCpu, "BMI", addressing_mode_rel_8, operation_function_BMI, cycle_count_mode02, 2, 0x30, 2, 0);
    cpu_add_op_code(myCpu, "AND", addressing_mode_dir_y_16bit, operation_function_AND, cycle_count_mode0A, 7, 0x31, 2, 0);
    cpu_add_op_code(myCpu, "AND", addressing_mode_dir_16bit, operation_function_AND, cycle_count_mode09, 6, 0x32, 2, 0);
    cpu_add_op_code(myCpu, "AND", addressing_mode_stk_s_y, operation_function_AND, cycle_count_mode08, 8, 0x33, 2, 0);
    cpu_add_op_code(myCpu, "BIT", addressing_mode_dir_offset_x, operation_function_BIT, cycle_count_mode09, 5, 0x34, 2, 0);
    cpu_add_op_code(myCpu, "AND", addressing_mode_dir_offset_x, operation_function_AND, cycle_count_mode09, 5, 0x35, 2, 0);
    cpu_add_op_code(myCpu, "ROL", addressing_mode_dir_offset_x, operation_function_ROL, cycle_count_mode05, 8, 0x36, 2, 0);
    cpu_add_op_code(myCpu, "AND", addressing_mode_dir_y_24bit, operation_function_AND, cycle_count_mode09, 7, 0x37, 2, 0);
    cpu_add_op_code(myCpu, "SEC", addressing_mode_imp, operation_function_SEC, cycle_count_mode00, 2, 0x38, 1, 0);
    cpu_add_op_code(myCpu, "AND", addressing_mode_abs_offset_y, operation_function_AND, cycle_count_mode0B, 6, 0x39, 3, 0);
    cpu_add_op_code(myCpu, "DEC", addressing_mode_acc, operation_function_DEC, cycle_count_mode00, 2, 0x3A, 1, 0);
    cpu_add_op_code(myCpu, "TSC", addressing_mode_imp, operation_function_TSC, cycle_count_mode00, 2, 0x3B, 1, 0);
    cpu_add_op_code(myCpu, "BIT", addressing_mode_abs_offset_x, operation_function_BIT, cycle_count_mode0B, 6, 0x3C, 3, 0);
    cpu_add_op_code(myCpu, "AND", addressing_mode_abs_offset_x, operation_function_AND, cycle_count_mode0B, 6, 0x3D, 3, 0);
    cpu_add_op_code(myCpu, "ROL", addressing_mode_abs_offset_x, operation_function_ROL, cycle_count_mode04, 9, 0x3E, 3, 0);
    cpu_add_op_code(myCpu, "AND", addressing_mode_long_x, operation_function_AND, cycle_count_mode08, 6, 0x3F, 4, 0);
    cpu_add_op_code(myCpu, "RTI", addressing_mode_imp, operation_function_RTI, cycle_count_mode07, 7, 0x40, 1, 0);
    cpu_add_op_code(myCpu, "EOR", addressing_mode_dir_offset_x_bank, operation_function_EOR, cycle_count_mode09, 7, 0x41, 2, 0);
    cpu_add_op_code(myCpu, "WDM", addressing_mode_imm, operation_function_WDM, cycle_count_mode00, 2, 0x42, 2, 0);
    cpu_add_op_code(myCpu, "EOR", addressing_mode_stk_s, operation_function_EOR, cycle_count_mode08, 5, 0x43, 2, 0);
    cpu_add_op_code(myCpu, "MVP", addressing_mode_src_dest, operation_function_MVP, cycle_count_mode00, 7, 0x44, 3, 0);
    cpu_add_op_code(myCpu, "EOR", addressing_mode_dir, operation_function_EOR, cycle_count_mode09, 4, 0x45, 2, 0);
    cpu_add_op_code(myCpu, "LSR", addressing_mode_dir, operation_function_LSR, cycle_count_mode05, 7, 0x46, 2, 0);
    cpu_add_op_code(myCpu, "EOR", addressing_mode_dir_24bit, operation_function_EOR, cycle_count_mode09, 7, 0x47, 2, 0);
    cpu_add_op_code(myCpu, "PHA", addressing_mode_imp, operation_function_PHA, cycle_count_mode08, 4, 0x48, 1, 0);
    cpu_add_op_code(myCpu, "EOR", addressing_mode_imm, operation_function_EOR, cycle_count_mode08, 3, 0x49, 3, 1);
    cpu_add_op_code(myCpu, "LSR", addressing_mode_acc, operation_function_LSR, cycle_count_mode00, 2, 0x4A, 1, 0);
    cpu_add_op_code(myCpu, "PHK", addressing_mode_imp, operation_function_PHK, cycle_count_mode00, 3, 0x4B, 1, 0);
    cpu_add_op_code(myCpu, "JMP", addressing_mode_abs, operation_function_JMP, cycle_count_mode00, 3, 0x4C, 3, 0);
    cpu_add_op_code(myCpu, "EOR", addressing_mode_abs, operation_function_EOR, cycle_count_mode08, 5, 0x4D, 3, 0);
    cpu_add_op_code(myCpu, "LSR", addressing_mode_abs, operation_function_LSR, cycle_count_mode04, 8, 0x4E, 3, 0);
    cpu_add_op_code(myCpu, "EOR", addressing_mode_long, operation_function_EOR, cycle_count_mode08, 6, 0x4F, 4, 0);
    cpu_add_op_code(myCpu, "BVC", addressing_mode_rel_8, operation_function_BVC, cycle_count_mode02, 2, 0x50, 2, 0);
    cpu_add_op_code(myCpu, "EOR", addressing_mode_dir_y_16bit, operation_function_EOR, cycle_count_mode0A, 7, 0x51, 2, 0);
    cpu_add_op_code(myCpu, "EOR", addressing_mode_dir_16bit, operation_function_EOR, cycle_count_mode09, 6, 0x52, 2, 0);
    cpu_add_op_code(myCpu, "EOR", addressing_mode_stk_s_y, operation_function_EOR, cycle_count_mode08, 8, 0x53, 2, 0);
    cpu_add_op_code(myCpu, "MVN", addressing_mode_src_dest, operation_function_MVN, cycle_count_mode00, 7, 0x54, 3, 0);
    cpu_add_op_code(myCpu, "EOR", addressing_mode_dir_offset_x, operation_function_EOR, cycle_count_mode09, 5, 0x55, 2, 0);
    cpu_add_op_code(myCpu, "LSR", addressing_mode_dir_offset_x, operation_function_LSR, cycle_count_mode05, 8, 0x56, 2, 0);
    cpu_add_op_code(myCpu, "EOR", addressing_mode_dir_y_24bit, operation_function_EOR, cycle_count_mode09, 7, 0x57, 2, 0);
    cpu_add_op_code(myCpu, "CLI", addressing_mode_imp, operation_function_CLI, cycle_count_mode00, 2, 0x58, 1, 0);
    cpu_add_op_code(myCpu, "EOR", addressing_mode_abs_offset_y, operation_function_EOR, cycle_count_mode0B, 6, 0x59, 3, 0);
    cpu_add_op_code(myCpu, "PHY", addressing_mode_imp, operation_function_PHY, cycle_count_mode0C, 4, 0x5A, 1, 0);
    cpu_add_op_code(myCpu, "TCD", addressing_mode_imp, operation_function_TCD, cycle_count_mode00, 2, 0x5B, 1, 0);
    cpu_add_op_code(myCpu, "JMP", addressing_mode_long, operation_function_JMP, cycle_count_mode00, 4, 0x5C, 4, 0);
    cpu_add_op_code(myCpu, "EOR", addressing_mode_abs_offset_x, operation_function_EOR, cycle_count_mode0B, 6, 0x5D, 3, 0);
    cpu_add_op_code(myCpu, "LSR", addressing_mode_abs_offset_x, operation_function_LSR, cycle_count_mode04, 9, 0x5E, 3, 0);
    cpu_add_op_code(myCpu, "EOR", addressing_mode_long_x, operation_function_EOR, cycle_count_mode08, 6, 0x5F, 4, 0);
    cpu_add_op_code(myCpu, "RTS", addressing_mode_imp, operation_function_RTS, cycle_count_mode00, 6, 0x60, 1, 0);
    cpu_add_op_code(myCpu, "ADC", addressing_mode_dir_offset_x_bank, operation_function_ADC, cycle_count_mode09, 7, 0x61, 2, 0);
    cpu_add_op_code(myCpu, "PER", addressing_mode_imm, operation_function_PER, cycle_count_mode00, 6, 0x62, 3, 0);
    cpu_add_op_code(myCpu, "ADC", addressing_mode_stk_s, operation_function_ADC, cycle_count_mode08, 5, 0x63, 2, 0);
    cpu_add_op_code(myCpu, "STZ", addressing_mode_dir, operation_function_STZ, cycle_count_mode09, 4, 0x64, 2, 0);
    cpu_add_op_code(myCpu, "ADC", addressing_mode_dir, operation_function_ADC, cycle_count_mode09, 4, 0x65, 2, 0);
    cpu_add_op_code(myCpu, "ROR", addressing_mode_dir, operation_function_ROR, cycle_count_mode05, 7, 0x66, 2, 0);
    cpu_add_op_code(myCpu, "ADC", addressing_mode_dir_24bit, operation_function_ADC, cycle_count_mode09, 7, 0x67, 2, 0);
    cpu_add_op_code(myCpu, "PLA", addressing_mode_imp, operation_function_PLA, cycle_count_mode08, 5, 0x68, 1, 0);
    cpu_add_op_code(myCpu, "ADC", addressing_mode_imm, operation_function_ADC, cycle_count_mode08, 3, 0x69, 3, 1);
    cpu_add_op_code(myCpu, "ROR", addressing_mode_acc, operation_function_ROR, cycle_count_mode00, 2, 0x6A, 1, 0);
    cpu_add_op_code(myCpu, "RTL", addressing_mode_imp, operation_function_RTL, cycle_count_mode00, 6, 0x6B, 1, 0);
    cpu_add_op_code(myCpu, "JMP", addressing_mode_abs_16bit, operation_function_JMP, cycle_count_mode00, 5, 0x6C, 3, 0);
    cpu_add_op_code(myCpu, "ADC", addressing_mode_abs, operation_function_ADC, cycle_count_mode08, 5, 0x6D, 3, 0);
    cpu_add_op_code(myCpu, "ROR", addressing_mode_abs, operation_function_ROR, cycle_count_mode04, 8, 0x6E, 3, 0);
    cpu_add_op_code(myCpu, "ADC", addressing_mode_long, operation_function_ADC, cycle_count_mode08, 6, 0x6F, 4, 0);
    cpu_add_op_code(myCpu, "BVS", addressing_mode_rel_8, operation_function_BVS, cycle_count_mode02, 2, 0x70, 2, 0);
    cpu_add_op_code(myCpu, "ADC", addressing_mode_dir_y_16bit, operation_function_ADC, cycle_count_mode0A, 7, 0x71, 2, 0);
    cpu_add_op_code(myCpu, "ADC", addressing_mode_dir_16bit, operation_function_ADC, cycle_count_mode09, 6, 0x72, 2, 0);
    cpu_add_op_code(myCpu, "ADC", addressing_mode_stk_s_y, operation_function_ADC, cycle_count_mode08, 8, 0x73, 2, 0);
    cpu_add_op_code(myCpu, "STZ", addressing_mode_dir_offset_x, operation_function_STZ, cycle_count_mode09, 5, 0x74, 2, 0);
    cpu_add_op_code(myCpu, "ADC", addressing_mode_dir_offset_x, operation_function_ADC, cycle_count_mode09, 5, 0x75, 2, 0);
    cpu_add_op_code(myCpu, "ROR", addressing_mode_dir_offset_x, operation_function_ROR, cycle_count_mode05, 8, 0x76, 2, 0);
    cpu_add_op_code(myCpu, "ADC", addressing_mode_dir_y_24bit, operation_function_ADC, cycle_count_mode09, 7, 0x77, 2, 0);
    cpu_add_op_code(myCpu, "SEI", addressing_mode_imp, operation_function_SEI, cycle_count_mode00, 2, 0x78, 1, 0);
    cpu_add_op_code(myCpu, "ADC", addressing_mode_abs_offset_y, operation_function_ADC, cycle_count_mode0B, 6, 0x79, 3, 0);
    cpu_add_op_code(myCpu, "PLY", addressing_mode_imp, operation_function_PLY, cycle_count_mode0C, 5, 0x7A, 1, 0);
    cpu_add_op_code(myCpu, "TDC", addressing_mode_imp, operation_function_TDC, cycle_count_mode00, 2, 0x7B, 1, 0);
    cpu_add_op_code(myCpu, "JMP", addressing_mode_abs_offset_x_bank, operation_function_JMP, cycle_count_mode00, 6, 0x7C, 3, 0);
    cpu_add_op_code(myCpu, "ADC", addressing_mode_abs_offset_x, operation_function_ADC, cycle_count_mode0B, 6, 0x7D, 3, 0);
    cpu_add_op_code(myCpu, "ROR", addressing_mode_abs_offset_x, operation_function_ROR, cycle_count_mode04, 9, 0x7E, 3, 0);
    cpu_add_op_code(myCpu, "ADC", addressing_mode_long_x, operation_function_ADC, cycle_count_mode08, 6, 0x7F, 4, 0);
    cpu_add_op_code(myCpu, "BRA", addressing_mode_rel_8, operation_function_BRA, cycle_count_mode01, 3, 0x80, 2, 0);
    cpu_add_op_code(myCpu, "STA", addressing_mode_dir_offset_x_bank, operation_function_STA, cycle_count_mode09, 7, 0x81, 2, 0);
    cpu_add_op_code(myCpu, "BRL", addressing_mode_rel_16, operation_function_BRL, cycle_count_mode00, 4, 0x82, 3, 0);
    cpu_add_op_code(myCpu, "STA", addressing_mode_stk_s, operation_function_STA, cycle_count_mode08, 5, 0x83, 2, 0);
    cpu_add_op_code(myCpu, "STY", addressing_mode_dir, operation_function_STY, cycle_count_mode0D, 4, 0x84, 2, 0);
    cpu_add_op_code(myCpu, "STA", addressing_mode_dir, operation_function_STA, cycle_count_mode09, 4, 0x85, 2, 0);
    cpu_add_op_code(myCpu, "STX", addressing_mode_dir, operation_function_STX, cycle_count_mode0D, 4, 0x86, 2, 0);
    cpu_add_op_code(myCpu, "STA", addressing_mode_dir_24bit, operation_function_STA, cycle_count_mode09, 7, 0x87, 2, 0);
    cpu_add_op_code(myCpu, "DEY", addressing_mode_imp, operation_function_DEY, cycle_count_mode00, 2, 0x88, 1, 0);
    cpu_add_op_code(myCpu, "BIT", addressing_mode_imm, operation_function_BIT, cycle_count_mode08, 3, 0x89, 3, 1);
    cpu_add_op_code(myCpu, "TXA", addressing_mode_imp, operation_function_TXA, cycle_count_mode00, 2, 0x8A, 1, 0);
    cpu_add_op_code(myCpu, "PHB", addressing_mode_imp, operation_function_PHB, cycle_count_mode00, 3, 0x8B, 1, 0);
    cpu_add_op_code(myCpu, "STY", addressing_mode_abs, operation_function_STY, cycle_count_mode0C, 5, 0x8C, 3, 0);
    cpu_add_op_code(myCpu, "STA", addressing_mode_abs, operation_function_STA, cycle_count_mode08, 5, 0x8D, 3, 0);
    cpu_add_op_code(myCpu, "STX", addressing_mode_abs, operation_function_STX, cycle_count_mode0C, 5, 0x8E, 3, 0);
    cpu_add_op_code(myCpu, "STA", addressing_mode_long, operation_function_STA, cycle_count_mode08, 6, 0x8F, 4, 0);
    cpu_add_op_code(myCpu, "BCC", addressing_mode_rel_8, operation_function_BCC, cycle_count_mode02, 2, 0x90, 2, 0);
    cpu_add_op_code(myCpu, "STA", addressing_mode_dir_y_16bit, operation_function_STA, cycle_count_mode09, 7, 0x91, 2, 0);
    cpu_add_op_code(myCpu, "STA", addressing_mode_dir_16bit, operation_function_STA, cycle_count_mode09, 6, 0x92, 2, 0);
    cpu_add_op_code(myCpu, "STA", addressing_mode_stk_s_y, operation_function_STA, cycle_count_mode08, 8, 0x93, 2, 0);
    cpu_add_op_code(myCpu, "STY", addressing_mode_dir_offset_x, operation_function_STY, cycle_count_mode0D, 5, 0x94, 2, 0);
    cpu_add_op_code(myCpu, "STA", addressing_mode_dir_offset_x, operation_function_STA, cycle_count_mode09, 5, 0x95, 2, 0);
    cpu_add_op_code(myCpu, "STX", addressing_mode_dir_offset_y, operation_function_STX, cycle_count_mode0D, 5, 0x96, 2, 0);
    cpu_add_op_code(myCpu, "STA", addressing_mode_dir_y_24bit, operation_function_STA, cycle_count_mode09, 7, 0x97, 2, 0);
    cpu_add_op_code(myCpu, "TYA", addressing_mode_imp, operation_function_TYA, cycle_count_mode00, 2, 0x98, 1, 0);
    cpu_add_op_code(myCpu, "STA", addressing_mode_abs_offset_y, operation_function_STA, cycle_count_mode08, 6, 0x99, 3, 0);
    cpu_add_op_code(myCpu, "TXS", addressing_mode_imp, operation_function_TXS, cycle_count_mode00, 2, 0x9A, 1, 0);
    cpu_add_op_code(myCpu, "TXY", addressing_mode_imp, operation_function_TXY, cycle_count_mode00, 2, 0x9B, 1, 0);
    cpu_add_op_code(myCpu, "STZ", addressing_mode_abs, operation_function_STZ, cycle_count_mode08, 5, 0x9C, 3, 0);
    cpu_add_op_code(myCpu, "STA", addressing_mode_abs_offset_x, operation_function_STA, cycle_count_mode08, 6, 0x9D, 3, 0);
    cpu_add_op_code(myCpu, "STZ", addressing_mode_abs_offset_x, operation_function_STZ, cycle_count_mode08, 6, 0x9E, 3, 0);
    cpu_add_op_code(myCpu, "STA", addressing_mode_long_x, operation_function_STA, cycle_count_mode08, 6, 0x9F, 4, 0);
    cpu_add_op_code(myCpu, "LDY", addressing_mode_imm, operation_function_LDY, cycle_count_mode0C, 3, 0xA0, 3, 2);
    cpu_add_op_code(myCpu, "LDA", addressing_mode_dir_offset_x_bank, operation_function_LDA, cycle_count_mode09, 7, 0xA1, 2, 0);
    cpu_add_op_code(myCpu, "LDX", addressing_mode_imm, operation_function_LDX, cycle_count_mode0C, 3, 0xA2, 3, 2);
    cpu_add_op_code(myCpu, "LDA", addressing_mode_stk_s, operation_function_LDA, cycle_count_mode08, 5, 0xA3, 2, 0);
    cpu_add_op_code(myCpu, "LDY", addressing_mode_dir, operation_function_LDY, cycle_count_mode0D, 4, 0xA4, 2, 0);
    cpu_add_op_code(myCpu, "LDA", addressing_mode_dir, operation_function_LDA, cycle_count_mode09, 4, 0xA5, 2, 0);
    cpu_add_op_code(myCpu, "LDX", addressing_mode_dir, operation_function_LDX, cycle_count_mode0D, 4, 0xA6, 2, 0);
    cpu_add_op_code(myCpu, "LDA", addressing_mode_dir_24bit, operation_function_LDA, cycle_count_mode09, 7, 0xA7, 2, 0);
    cpu_add_op_code(myCpu, "TAY", addressing_mode_imp, operation_function_TAY, cycle_count_mode00, 2, 0xA8, 1, 0);
    cpu_add_op_code(myCpu, "LDA", addressing_mode_imm, operation_function_LDA, cycle_count_mode08, 3, 0xA9, 3, 1);
    cpu_add_op_code(myCpu, "TAX", addressing_mode_imp, operation_function_TAX, cycle_count_mode00, 2, 0xAA, 1, 0);
    cpu_add_op_code(myCpu, "PLB", addressing_mode_imp, operation_function_PLB, cycle_count_mode00, 4, 0xAB, 1, 0);
    cpu_add_op_code(myCpu, "LDY", addressing_mode_abs, operation_function_LDY, cycle_count_mode0C, 5, 0xAC, 3, 0);
    cpu_add_op_code(myCpu, "LDA", addressing_mode_abs, operation_function_LDA, cycle_count_mode08, 5, 0xAD, 3, 0);
    cpu_add_op_code(myCpu, "LDX", addressing_mode_abs, operation_function_LDX, cycle_count_mode0C, 5, 0xAE, 3, 0);
    cpu_add_op_code(myCpu, "LDA", addressing_mode_long, operation_function_LDA, cycle_count_mode08, 6, 0xAF, 4, 0);
    cpu_add_op_code(myCpu, "BCS", addressing_mode_rel_8, operation_function_BCS, cycle_count_mode02, 2, 0xB0, 2, 0);
    cpu_add_op_code(myCpu, "LDA", addressing_mode_dir_y_16bit, operation_function_LDA, cycle_count_mode0A, 7, 0xB1, 2, 0);
    cpu_add_op_code(myCpu, "LDA", addressing_mode_dir_16bit, operation_function_LDA, cycle_count_mode09, 6, 0xB2, 2, 0);
    cpu_add_op_code(myCpu, "LDA", addressing_mode_stk_s_y, operation_function_LDA, cycle_count_mode08, 8, 0xB3, 2, 0);
    cpu_add_op_code(myCpu, "LDY", addressing_mode_dir_offset_x, operation_function_LDY, cycle_count_mode0D, 5, 0xB4, 2, 0);
    cpu_add_op_code(myCpu, "LDA", addressing_mode_dir_offset_x, operation_function_LDA, cycle_count_mode09, 5, 0xB5, 2, 0);
    cpu_add_op_code(myCpu, "LDX", addressing_mode_dir_offset_y, operation_function_LDX, cycle_count_mode0D, 5, 0xB6, 2, 0);
    cpu_add_op_code(myCpu, "LDA", addressing_mode_dir_y_24bit, operation_function_LDA, cycle_count_mode09, 7, 0xB7, 2, 0);
    cpu_add_op_code(myCpu, "CLV", addressing_mode_imp, operation_function_CLV, cycle_count_mode00, 2, 0xB8, 1, 0);
    cpu_add_op_code(myCpu, "LDA", addressing_mode_abs_offset_y, operation_function_LDA, cycle_count_mode0B, 6, 0xB9, 3, 0);
    cpu_add_op_code(myCpu, "TSX", addressing_mode_imp, operation_function_TSX, cycle_count_mode00, 2, 0xBA, 1, 0);
    cpu_add_op_code(myCpu, "TYX", addressing_mode_imp, operation_function_TYX, cycle_count_mode00, 2, 0xBB, 1, 0);
    cpu_add_op_code(myCpu, "LDY", addressing_mode_abs_offset_x, operation_function_LDY, cycle_count_mode06, 6, 0xBC, 3, 0);
    cpu_add_op_code(myCpu, "LDA", addressing_mode_abs_offset_x, operation_function_LDA, cycle_count_mode0B, 6, 0xBD, 3, 0);
    cpu_add_op_code(myCpu, "LDX", addressing_mode_abs_offset_y, operation_function_LDX, cycle_count_mode06, 6, 0xBE, 3, 0);
    cpu_add_op_code(myCpu, "LDA", addressing_mode_long_x, operation_function_LDA, cycle_count_mode08, 6, 0xBF, 4, 0);
    cpu_add_op_code(myCpu, "CPY", addressing_mode_imm, operation_function_CPY, cycle_count_mode0C, 3, 0xC0, 3, 2);
    cpu_add_op_code(myCpu, "CMP", addressing_mode_dir_offset_x_bank, operation_function_CMP, cycle_count_mode09, 7, 0xC1, 2, 0);
    cpu_add_op_code(myCpu, "REP", addressing_mode_imm, operation_function_REP, cycle_count_mode00, 3, 0xC2, 2, 0);
    cpu_add_op_code(myCpu, "CMP", addressing_mode_stk_s, operation_function_CMP, cycle_count_mode08, 5, 0xC3, 2, 0);
    cpu_add_op_code(myCpu, "CPY", addressing_mode_dir, operation_function_CPY, cycle_count_mode0D, 4, 0xC4, 2, 0);
    cpu_add_op_code(myCpu, "CMP", addressing_mode_dir, operation_function_CMP, cycle_count_mode09, 4, 0xC5, 2, 0);
    cpu_add_op_code(myCpu, "DEC", addressing_mode_dir, operation_function_DEC, cycle_count_mode05, 7, 0xC6, 2, 0);
    cpu_add_op_code(myCpu, "CMP", addressing_mode_dir_24bit, operation_function_CMP, cycle_count_mode09, 7, 0xC7, 2, 0);
    cpu_add_op_code(myCpu, "INY", addressing_mode_imp, operation_function_INY, cycle_count_mode00, 2, 0xC8, 1, 0);
    cpu_add_op_code(myCpu, "CMP", addressing_mode_imm, operation_function_CMP, cycle_count_mode08, 3, 0xC9, 3, 1);
    cpu_add_op_code(myCpu, "DEX", addressing_mode_imp, operation_function_DEX, cycle_count_mode00, 2, 0xCA, 1, 0);
    cpu_add_op_code(myCpu, "WAI", addressing_mode_imp, operation_function_WAI, cycle_count_mode00, 3, 0xCB, 1, 0);
    cpu_add_op_code(myCpu, "CPY", addressing_mode_abs, operation_function_CPY, cycle_count_mode0C, 5, 0xCC, 3, 0);
    cpu_add_op_code(myCpu, "CMP", addressing_mode_abs, operation_function_CMP, cycle_count_mode08, 5, 0xCD, 3, 0);
    cpu_add_op_code(myCpu, "DEC", addressing_mode_abs, operation_function_DEC, cycle_count_mode04, 8, 0xCE, 3, 0);
    cpu_add_op_code(myCpu, "CMP", addressing_mode_long, operation_function_CMP, cycle_count_mode08, 6, 0xCF, 4, 0);
    cpu_add_op_code(myCpu, "BNE", addressing_mode_rel_8, operation_function_BNE, cycle_count_mode02, 2, 0xD0, 2, 0);
    cpu_add_op_code(myCpu, "CMP", addressing_mode_dir_y_16bit, operation_function_CMP, cycle_count_mode0A, 7, 0xD1, 2, 0);
    cpu_add_op_code(myCpu, "CMP", addressing_mode_dir_16bit, operation_function_CMP, cycle_count_mode09, 6, 0xD2, 2, 0);
    cpu_add_op_code(myCpu, "CMP", addressing_mode_stk_s_y, operation_function_CMP, cycle_count_mode08, 8, 0xD3, 2, 0);
    cpu_add_op_code(myCpu, "PEI", addressing_mode_dir, operation_function_PEI, cycle_count_mode03, 6, 0xD4, 2, 0);
    cpu_add_op_code(myCpu, "CMP", addressing_mode_dir_offset_x, operation_function_CMP, cycle_count_mode09, 5, 0xD5, 2, 0);
    cpu_add_op_code(myCpu, "DEC", addressing_mode_dir_offset_x, operation_function_DEC, cycle_count_mode05, 8, 0xD6, 2, 0);
    cpu_add_op_code(myCpu, "CMP", addressing_mode_dir_y_24bit, operation_function_CMP, cycle_count_mode09, 7, 0xD7, 2, 0);
    cpu_add_op_code(myCpu, "CLD", addressing_mode_imp, operation_function_CLD, cycle_count_mode00, 2, 0xD8, 1, 0);
    cpu_add_op_code(myCpu, "CMP", addressing_mode_abs_offset_y, operation_function_CMP, cycle_count_mode0B, 6, 0xD9, 3, 0);
    cpu_add_op_code(myCpu, "PHX", addressing_mode_imp, operation_function_PHX, cycle_count_mode0C, 4, 0xDA, 1, 0);
    cpu_add_op_code(myCpu, "STP", addressing_mode_imp, operation_function_STP, cycle_count_mode00, 3, 0xDB, 1, 0);
    cpu_add_op_code(myCpu, "JMP", addressing_mode_abs_24bit, operation_function_JMP, cycle_count_mode00, 6, 0xDC, 3, 0);
    cpu_add_op_code(myCpu, "CMP", addressing_mode_abs_offset_x, operation_function_CMP, cycle_count_mode0B, 6, 0xDD, 3, 0);
    cpu_add_op_code(myCpu, "DEC", addressing_mode_abs_offset_x, operation_function_DEC, cycle_count_mode04, 9, 0xDE, 3, 0);
    cpu_add_op_code(myCpu, "CMP", addressing_mode_long_x, operation_function_CMP, cycle_count_mode08, 6, 0xDF, 4, 0);
    cpu_add_op_code(myCpu, "CPX", addressing_mode_imm, operation_function_CPX, cycle_count_mode0C, 3, 0xE0, 3, 2);
    cpu_add_op_code(myCpu, "SBC", addressing_mode_dir_offset_x_bank, operation_function_SBC, cycle_count_mode09, 7, 0xE1, 2, 0);
    cpu_add_op_code(myCpu, "SEP", addressing_mode_imm, operation_function_SEP, cycle_count_mode00, 3, 0xE2, 2, 0);
    cpu_add_op_code(myCpu, "SBC", addressing_mode_stk_s, operation_function_SBC, cycle_count_mode08, 5, 0xE3, 2, 0);
    cpu_add_op_code(myCpu, "CPX", addressing_mode_dir, operation_function_CPX, cycle_count_mode0D, 4, 0xE4, 2, 0);
    cpu_add_op_code(myCpu, "SBC", addressing_mode_dir, operation_function_SBC, cycle_count_mode09, 4, 0xE5, 2, 0);
    cpu_add_op_code(myCpu, "INC", addressing_mode_dir, operation_function_INC, cycle_count_mode05, 7, 0xE6, 2, 0);
    cpu_add_op_code(myCpu, "SBC", addressing_mode_dir_24bit, operation_function_SBC, cycle_count_mode09, 7, 0xE7, 2, 0);
    cpu_add_op_code(myCpu, "INX", addressing_mode_imp, operation_function_INX, cycle_count_mode00, 2, 0xE8, 1, 0);
    cpu_add_op_code(myCpu, "SBC", addressing_mode_imm, operation_function_SBC, cycle_count_mode08, 3, 0xE9, 3, 1);
    cpu_add_op_code(myCpu, "NOP", addressing_mode_imp, operation_function_NOP, cycle_count_mode00, 2, 0xEA, 1, 0);
    cpu_add_op_code(myCpu, "XBA", addressing_mode_imp, operation_function_XBA, cycle_count_mode00, 3, 0xEB, 1, 0);
    cpu_add_op_code(myCpu, "CPX", addressing_mode_abs, operation_function_CPX, cycle_count_mode0C, 5, 0xEC, 3, 0);
    cpu_add_op_code(myCpu, "SBC", addressing_mode_abs, operation_function_SBC, cycle_count_mode08, 5, 0xED, 3, 0);
    cpu_add_op_code(myCpu, "INC", addressing_mode_abs, operation_function_INC, cycle_count_mode04, 8, 0xEE, 3, 0);
    cpu_add_op_code(myCpu, "SBC", addressing_mode_long, operation_function_SBC, cycle_count_mode08, 6, 0xEF, 4, 0);
    cpu_add_op_code(myCpu, "BEQ", addressing_mode_rel_8, operation_function_BEQ, cycle_count_mode02, 2, 0xF0, 2, 0);
    cpu_add_op_code(myCpu, "SBC", addressing_mode_dir_y_16bit, operation_function_SBC, cycle_count_mode0A, 7, 0xF1, 2, 0);
    cpu_add_op_code(myCpu, "SBC", addressing_mode_dir_16bit, operation_function_SBC, cycle_count_mode09, 6, 0xF2, 2, 0);
    cpu_add_op_code(myCpu, "SBC", addressing_mode_stk_s_y, operation_function_SBC, cycle_count_mode08, 8, 0xF3, 2, 0);
    cpu_add_op_code(myCpu, "PEA", addressing_mode_imm, operation_function_PEA, cycle_count_mode00, 5, 0xF4, 3, 0);
    cpu_add_op_code(myCpu, "SBC", addressing_mode_dir_offset_x, operation_function_SBC, cycle_count_mode09, 5, 0xF5, 2, 0);
    cpu_add_op_code(myCpu, "INC", addressing_mode_dir_offset_x, operation_function_INC, cycle_count_mode05, 8, 0xF6, 2, 0);
    cpu_add_op_code(myCpu, "SBC", addressing_mode_dir_y_24bit, operation_function_SBC, cycle_count_mode09, 7, 0xF7, 2, 0);
    cpu_add_op_code(myCpu, "SED", addressing_mode_imp, operation_function_SED, cycle_count_mode00, 2, 0xF8, 1, 0);
    cpu_add_op_code(myCpu, "SBC", addressing_mode_abs_offset_y, operation_function_SBC, cycle_count_mode0B, 6, 0xF9, 3, 0);
    cpu_add_op_code(myCpu, "PLX", addressing_mode_imp, operation_function_PLX, cycle_count_mode0C, 5, 0xFA, 1, 0);
    cpu_add_op_code(myCpu, "XCE", addressing_mode_imp, operation_function_XCE, cycle_count_mode00, 2, 0xFB, 1, 0);
    cpu_add_op_code(myCpu, "JSR", addressing_mode_abs_offset_x_bank, operation_function_JSR, cycle_count_mode00, 8, 0xFC, 3, 0);
    cpu_add_op_code(myCpu, "SBC", addressing_mode_abs_offset_x, operation_function_SBC, cycle_count_mode0B, 6, 0xFD, 3, 0);
    cpu_add_op_code(myCpu, "INC", addressing_mode_abs_offset_x, operation_function_INC, cycle_count_mode04, 9, 0xFE, 3, 0);
    cpu_add_op_code(myCpu, "SBC", addressing_mode_long_x, operation_function_SBC, cycle_count_mode08, 6, 0xFF, 4, 0);
}