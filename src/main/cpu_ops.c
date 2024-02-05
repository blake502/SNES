#include "cpu.h"

//I have no words

#pragma region Cycles count modes
//These cycle count mode calculations are named arbitrarily.
//They are necessary for accurate cycle calculations.
//There are only 13 cycle calculation modes.
//This may be refactored at some point, but it's functional as it is.

//Absolute
u8 cycle_count_mode_00(cpu* myCpu, u8 absolute, u8 page_boundary_crossed, u8 branch_taken)
{
    return absolute;
}

//Absolute+e*p
u8 cycle_count_mode_01(cpu* myCpu, u8 absolute, u8 page_boundary_crossed, u8 branch_taken)
{
    return absolute+myCpu->flag_e*page_boundary_crossed;
}

//Absolute+t+t*e*p
u8 cycle_count_mode_02(cpu* myCpu, u8 absolute, u8 page_boundary_crossed, u8 branch_taken)
{
    return absolute+branch_taken+branch_taken*myCpu->flag_e*page_boundary_crossed;
}

//Absolute+w
u8 cycle_count_mode_03(cpu* myCpu, u8 absolute, u8 page_boundary_crossed, u8 branch_taken)
{
    u8 w = myCpu->register_dl == 0 ? 0 : 1;
    return absolute+w;
}

//Absolute-2*m
u8 cycle_count_mode_04(cpu* myCpu, u8 absolute, u8 page_boundary_crossed, u8 branch_taken)
{
    return absolute-2*myCpu->flag_m;
}

//Absolute-2*m+w
u8 cycle_count_mode_05(cpu* myCpu, u8 absolute, u8 page_boundary_crossed, u8 branch_taken)
{
    u8 w = myCpu->register_dl == 0 ? 0 : 1;
    return absolute-2*myCpu->flag_m+w;
}

//Absolute-2*x+x*p
u8 cycle_count_mode_06(cpu* myCpu, u8 absolute, u8 page_boundary_crossed, u8 branch_taken)
{
    return absolute-2*myCpu->flag_x+myCpu->flag_x*page_boundary_crossed;
}

//Absolute-e
u8 cycle_count_mode_07(cpu* myCpu, u8 absolute, u8 page_boundary_crossed, u8 branch_taken)
{
    return absolute-myCpu->flag_e;
}

//Absolute-m
u8 cycle_count_mode_08(cpu* myCpu, u8 absolute, u8 page_boundary_crossed, u8 branch_taken)
{
    return absolute-myCpu->flag_m;
}

//Absolute-m+w
u8 cycle_count_mode_09(cpu* myCpu, u8 absolute, u8 page_boundary_crossed, u8 branch_taken)
{
    u8 w = myCpu->register_dl == 0 ? 0 : 1;
    return absolute-myCpu->flag_m+w;
}

//Absolute-m+w-x+x*p
u8 cycle_count_mode_0A(cpu* myCpu, u8 absolute, u8 page_boundary_crossed, u8 branch_taken)
{
    u8 w = myCpu->register_dl == 0 ? 0 : 1;
    return absolute-myCpu->flag_m+w-myCpu->flag_x+myCpu->flag_x*page_boundary_crossed;
}

//Absolute-m-x+x*p
u8 cycle_count_mode_0B(cpu* myCpu, u8 absolute, u8 page_boundary_crossed, u8 branch_taken)
{
    return absolute-myCpu->flag_m-myCpu->flag_x+myCpu->flag_x*page_boundary_crossed;
}

//Absolute-x
u8 cycle_count_mode_0C(cpu* myCpu, u8 absolute, u8 page_boundary_crossed, u8 branch_taken)
{
    return absolute-myCpu->flag_x;
}

//Absolute-x+w
u8 cycle_count_mode_0D(cpu* myCpu, u8 absolute, u8 page_boundary_crossed, u8 branch_taken)
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
    return 0;
}

//ABSOLUTE,X
//Length: 3 bytes, $OP $LL $HH
u8 addressing_mode_abs_offset_x(cpu* myCpu)
{
    return 0;
}

//ABSOLUTE,Y
//Length: 3 bytes, $OP $LL $HH
u8 addressing_mode_abs_offset_y(cpu* myCpu)
{
    return 0;
}

//(ABSOLUTE)
//Length: 3 bytes, $OP $LL $HH 
//(absolute) addressing uses a 16-bit pointer.
u8 addressing_mode_abs_16bit(cpu* myCpu)
{
    return 0;
}

//[ABSOLUTE]
//Length: 3 bytes, $OP $LL $HH 
//[absolute] addressing uses a 24-bit pointer.
u8 addressing_mode_abs_24bit(cpu* myCpu)
{
    return 0;
}

//(ABSOLUTE,X)
//Length: 3 bytes, $OP $LL $HH 
//(absolute,X) addressing uses a 16-bit pointer
u8 addressing_mode_abs_offset_x_bank(cpu* myCpu)
{
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
    return 0;
}

// DIRECT,X
//Length: 2 bytes, $OP $LL 
u8 addressing_mode_dir_offset_x(cpu* myCpu)
{
    return 0;
}

// DIRECT,Y
//Length: 2 bytes, $OP $LL 
u8 addressing_mode_dir_offset_y(cpu* myCpu)
{
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
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_AND(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_ASL(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_BCC(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_BCS(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_BEQ(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_BIT(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_BMI(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_BNE(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_BPL(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_BRA(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_BRK(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_BRL(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_BVC(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_BVS(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

//Clears the c flag
u8 operation_function_CLC(cpu* myCpu)
{
    myCpu->flag_c = 0;
    myCpu->program_counter++;
    return 0;
}

//Clears the d flag
u8 operation_function_CLD(cpu* myCpu)
{
    myCpu->flag_d = 0;
    myCpu->program_counter++;
    return 0;
}

//Clears the i flag
u8 operation_function_CLI(cpu* myCpu)
{
    myCpu->flag_i = 0;
    myCpu->program_counter++;
    return 0;
}

//Clears the v flag
u8 operation_function_CLV(cpu* myCpu)
{
    myCpu->flag_v = 0;
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_CMP(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_COP(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_CPX(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_CPY(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_DEC(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_DEX(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_DEY(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_EOR(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_INC(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_INX(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_INY(cpu* myCpu)
{
    myCpu->program_counter++;
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
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_JSR(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_LDA(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_LDX(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_LDY(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_LSR(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_MVN(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_MVP(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_NOP(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_ORA(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_PEA(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_PEI(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_PER(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_PHA(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_PHB(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_PHD(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_PHK(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_PHP(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_PHX(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_PHY(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_PLA(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_PLB(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_PLD(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_PLP(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_PLX(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_PLY(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

//REset Processor status bits
u8 operation_function_REP(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_ROL(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_ROR(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_RTI(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_RTL(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_RTS(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_SBC(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

//Sets the c flag
u8 operation_function_SEC(cpu* myCpu)
{
    myCpu->flag_c = 1;
    myCpu->program_counter++;
    return 0;
}

//Sets the d flag
u8 operation_function_SED(cpu* myCpu)
{
    myCpu->flag_d = 1;
    myCpu->program_counter++;
    return 0;
}

//Sets the i flag
u8 operation_function_SEI(cpu* myCpu)
{
    myCpu->flag_i = 1;
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_SEP(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_STA(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_STP(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_STX(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_STY(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_STZ(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_TAX(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_TAY(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_TCD(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_TCS(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_TDC(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_TRB(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_TSB(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_TSC(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_TSX(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_TXA(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_TXS(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_TXY(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_TYA(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_TYX(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_WAI(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_WDM(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

u8 operation_function_XBA(cpu* myCpu)
{
    myCpu->program_counter++;
    return 0;
}

//Exchange Carry and Emulation flags
u8 operation_function_XCE(cpu* myCpu)
{
    u8 temp = myCpu->flag_c;
    myCpu->flag_c = myCpu->flag_x;
    myCpu->flag_x = temp;
    myCpu->program_counter++;
    return 0;
}

#pragma endregion

#pragma region Op code registration
//Registers an op code
void cpu_register_op_code(
    cpu* myCpu,
    u16 opcode,
    const char* name,
    u8 (*addressing_mode_func_ptr)(cpu* myCpu),
    u8 (*operation_func_ptr)(cpu* myCpu),
    u8 (*cycles_calculation_func_ptr)(cpu* myCpu, u8 absolute, u8 page_boundary_crossed, u8 branch_taken),
    u8 base_cycles,
    u8 base_length,
    u8 length_mode
    )
{
    //Create an op code and assign all the information
    op_code myOp_code;
    myOp_code.instruction_name = name;
    myOp_code.addressing_mode_func_ptr = addressing_mode_func_ptr;
    myOp_code.operation_func_ptr = operation_func_ptr;
    myOp_code.cycles_calculation_func_ptr = cycles_calculation_func_ptr;
    myOp_code.base_cycles = base_cycles;
    myOp_code.base_length = base_length;
    myOp_code.length_mode = length_mode;

    //Store the op code in the CPU's registration of all op codes
    myCpu->op_codes[opcode] = myOp_code;
}

//Registers all op codes
void cpu_register_all_op_codes(cpu* myCpu)
{
    /*
    cpu_register_op_code(myCpu, 0x0, "XXX", addressing_mode_XXX, operation_function_XXX, cycle_count_mode_XX, CYCLES, LENGTH, LENGTH_MODE);
    */
    cpu_register_op_code(myCpu, 0x00, "BRK", addressing_mode_imp              , operation_function_BRK, cycle_count_mode_07, 8, 1, 0);
    cpu_register_op_code(myCpu, 0x01, "ORA", addressing_mode_dir_offset_x_bank, operation_function_ORA, cycle_count_mode_09, 7, 2, 0);
    cpu_register_op_code(myCpu, 0x02, "COP", addressing_mode_imm              , operation_function_COP, cycle_count_mode_07, 8, 2, 0);
    cpu_register_op_code(myCpu, 0x03, "ORA", addressing_mode_stk_s            , operation_function_ORA, cycle_count_mode_08, 5, 2, 0);
    cpu_register_op_code(myCpu, 0x04, "TSB", addressing_mode_dir              , operation_function_TSB, cycle_count_mode_05, 7, 2, 0);
    cpu_register_op_code(myCpu, 0x05, "ORA", addressing_mode_dir              , operation_function_ORA, cycle_count_mode_09, 4, 2, 0);
    cpu_register_op_code(myCpu, 0x06, "ASL", addressing_mode_dir              , operation_function_ASL, cycle_count_mode_05, 7, 2, 0);
    cpu_register_op_code(myCpu, 0x07, "ORA", addressing_mode_dir_24bit        , operation_function_ORA, cycle_count_mode_09, 7, 2, 0);
    cpu_register_op_code(myCpu, 0x08, "PHP", addressing_mode_imp              , operation_function_PHP, cycle_count_mode_00, 3, 1, 0);
    cpu_register_op_code(myCpu, 0x09, "ORA", addressing_mode_imm              , operation_function_ORA, cycle_count_mode_08, 3, 3, 1);
    cpu_register_op_code(myCpu, 0x0A, "ASL", addressing_mode_acc              , operation_function_ASL, cycle_count_mode_00, 2, 1, 0);
    cpu_register_op_code(myCpu, 0x0B, "PHD", addressing_mode_imp              , operation_function_PHD, cycle_count_mode_00, 4, 1, 0);
    cpu_register_op_code(myCpu, 0x0C, "TSB", addressing_mode_abs              , operation_function_TSB, cycle_count_mode_04, 8, 3, 0);
    cpu_register_op_code(myCpu, 0x0D, "ORA", addressing_mode_abs              , operation_function_ORA, cycle_count_mode_08, 5, 3, 0);
    cpu_register_op_code(myCpu, 0x0E, "ASL", addressing_mode_abs              , operation_function_ASL, cycle_count_mode_04, 8, 3, 0);
    cpu_register_op_code(myCpu, 0x0F, "ORA", addressing_mode_long             , operation_function_ORA, cycle_count_mode_08, 6, 4, 0);
    cpu_register_op_code(myCpu, 0x10, "BPL", addressing_mode_rel_8            , operation_function_BPL, cycle_count_mode_02, 2, 2, 0);
    cpu_register_op_code(myCpu, 0x11, "ORA", addressing_mode_dir_y_16bit      , operation_function_ORA, cycle_count_mode_0A, 7, 2, 0);
    cpu_register_op_code(myCpu, 0x12, "ORA", addressing_mode_dir_16bit        , operation_function_ORA, cycle_count_mode_09, 6, 2, 0);
    cpu_register_op_code(myCpu, 0x13, "ORA", addressing_mode_stk_s_y          , operation_function_ORA, cycle_count_mode_08, 8, 2, 0);
    cpu_register_op_code(myCpu, 0x14, "TRB", addressing_mode_dir              , operation_function_TRB, cycle_count_mode_05, 7, 2, 0);
    cpu_register_op_code(myCpu, 0x15, "ORA", addressing_mode_dir_offset_x     , operation_function_ORA, cycle_count_mode_09, 5, 2, 0);
    cpu_register_op_code(myCpu, 0x16, "ASL", addressing_mode_dir_offset_x     , operation_function_ASL, cycle_count_mode_05, 8, 2, 0);
    cpu_register_op_code(myCpu, 0x17, "ORA", addressing_mode_dir_y_24bit      , operation_function_ORA, cycle_count_mode_09, 7, 2, 0);
    cpu_register_op_code(myCpu, 0x18, "CLC", addressing_mode_imp              , operation_function_CLC, cycle_count_mode_00, 2, 1, 0);
    cpu_register_op_code(myCpu, 0x19, "ORA", addressing_mode_abs_offset_y     , operation_function_ORA, cycle_count_mode_0B, 6, 3, 0);
    cpu_register_op_code(myCpu, 0x1A, "INC", addressing_mode_acc              , operation_function_INC, cycle_count_mode_00, 2, 1, 0);
    cpu_register_op_code(myCpu, 0x1B, "TCS", addressing_mode_imp              , operation_function_TCS, cycle_count_mode_00, 2, 1, 0);
    cpu_register_op_code(myCpu, 0x1C, "TRB", addressing_mode_abs              , operation_function_TRB, cycle_count_mode_04, 8, 3, 0);
    cpu_register_op_code(myCpu, 0x1D, "ORA", addressing_mode_abs_offset_x     , operation_function_ORA, cycle_count_mode_0B, 6, 3, 0);
    cpu_register_op_code(myCpu, 0x1E, "ASL", addressing_mode_abs_offset_x     , operation_function_ASL, cycle_count_mode_04, 9, 3, 0);
    cpu_register_op_code(myCpu, 0x1F, "ORA", addressing_mode_long_x           , operation_function_ORA, cycle_count_mode_08, 6, 4, 0);
    cpu_register_op_code(myCpu, 0x20, "JSR", addressing_mode_abs              , operation_function_JSR, cycle_count_mode_00, 6, 3, 0);
    cpu_register_op_code(myCpu, 0x21, "AND", addressing_mode_dir_offset_x_bank, operation_function_AND, cycle_count_mode_09, 7, 2, 0);
    cpu_register_op_code(myCpu, 0x22, "JSL", addressing_mode_long             , operation_function_JSL, cycle_count_mode_00, 8, 4, 0);
    cpu_register_op_code(myCpu, 0x23, "AND", addressing_mode_stk_s            , operation_function_AND, cycle_count_mode_08, 5, 2, 0);
    cpu_register_op_code(myCpu, 0x24, "BIT", addressing_mode_dir              , operation_function_BIT, cycle_count_mode_09, 4, 2, 0);
    cpu_register_op_code(myCpu, 0x25, "AND", addressing_mode_dir              , operation_function_AND, cycle_count_mode_09, 4, 2, 0);
    cpu_register_op_code(myCpu, 0x26, "ROL", addressing_mode_dir              , operation_function_ROL, cycle_count_mode_05, 7, 2, 0);
    cpu_register_op_code(myCpu, 0x27, "AND", addressing_mode_dir_24bit        , operation_function_AND, cycle_count_mode_09, 7, 2, 0);
    cpu_register_op_code(myCpu, 0x28, "PLP", addressing_mode_imp              , operation_function_PLP, cycle_count_mode_00, 4, 1, 0);
    cpu_register_op_code(myCpu, 0x29, "AND", addressing_mode_imm              , operation_function_AND, cycle_count_mode_08, 3, 3, 1);
    cpu_register_op_code(myCpu, 0x2A, "ROL", addressing_mode_acc              , operation_function_ROL, cycle_count_mode_00, 2, 1, 0);
    cpu_register_op_code(myCpu, 0x2B, "PLD", addressing_mode_imp              , operation_function_PLD, cycle_count_mode_00, 5, 1, 0);
    cpu_register_op_code(myCpu, 0x2C, "BIT", addressing_mode_abs              , operation_function_BIT, cycle_count_mode_08, 5, 3, 0);
    cpu_register_op_code(myCpu, 0x2D, "AND", addressing_mode_abs              , operation_function_AND, cycle_count_mode_08, 5, 3, 0);
    cpu_register_op_code(myCpu, 0x2E, "ROL", addressing_mode_abs              , operation_function_ROL, cycle_count_mode_04, 8, 3, 0);
    cpu_register_op_code(myCpu, 0x2F, "AND", addressing_mode_long             , operation_function_AND, cycle_count_mode_08, 6, 4, 0);
    cpu_register_op_code(myCpu, 0x30, "BMI", addressing_mode_rel_8            , operation_function_BMI, cycle_count_mode_02, 2, 2, 0);
    cpu_register_op_code(myCpu, 0x31, "AND", addressing_mode_dir_y_16bit      , operation_function_AND, cycle_count_mode_0A, 7, 2, 0);
    cpu_register_op_code(myCpu, 0x32, "AND", addressing_mode_dir_16bit        , operation_function_AND, cycle_count_mode_09, 6, 2, 0);
    cpu_register_op_code(myCpu, 0x33, "AND", addressing_mode_stk_s_y          , operation_function_AND, cycle_count_mode_08, 8, 2, 0);
    cpu_register_op_code(myCpu, 0x34, "BIT", addressing_mode_dir_offset_x     , operation_function_BIT, cycle_count_mode_09, 5, 2, 0);
    cpu_register_op_code(myCpu, 0x35, "AND", addressing_mode_dir_offset_x     , operation_function_AND, cycle_count_mode_09, 5, 2, 0);
    cpu_register_op_code(myCpu, 0x36, "ROL", addressing_mode_dir_offset_x     , operation_function_ROL, cycle_count_mode_05, 8, 2, 0);
    cpu_register_op_code(myCpu, 0x37, "AND", addressing_mode_dir_y_24bit      , operation_function_AND, cycle_count_mode_09, 7, 2, 0);
    cpu_register_op_code(myCpu, 0x38, "SEC", addressing_mode_imp              , operation_function_SEC, cycle_count_mode_00, 2, 1, 0);
    cpu_register_op_code(myCpu, 0x39, "AND", addressing_mode_abs_offset_y     , operation_function_AND, cycle_count_mode_0B, 6, 3, 0);
    cpu_register_op_code(myCpu, 0x3A, "DEC", addressing_mode_acc              , operation_function_DEC, cycle_count_mode_00, 2, 1, 0);
    cpu_register_op_code(myCpu, 0x3B, "TSC", addressing_mode_imp              , operation_function_TSC, cycle_count_mode_00, 2, 1, 0);
    cpu_register_op_code(myCpu, 0x3C, "BIT", addressing_mode_abs_offset_x     , operation_function_BIT, cycle_count_mode_0B, 6, 3, 0);
    cpu_register_op_code(myCpu, 0x3D, "AND", addressing_mode_abs_offset_x     , operation_function_AND, cycle_count_mode_0B, 6, 3, 0);
    cpu_register_op_code(myCpu, 0x3E, "ROL", addressing_mode_abs_offset_x     , operation_function_ROL, cycle_count_mode_04, 9, 3, 0);
    cpu_register_op_code(myCpu, 0x3F, "AND", addressing_mode_long_x           , operation_function_AND, cycle_count_mode_08, 6, 4, 0);
    cpu_register_op_code(myCpu, 0x40, "RTI", addressing_mode_imp              , operation_function_RTI, cycle_count_mode_07, 7, 1, 0);
    cpu_register_op_code(myCpu, 0x41, "EOR", addressing_mode_dir_offset_x_bank, operation_function_EOR, cycle_count_mode_09, 7, 2, 0);
    cpu_register_op_code(myCpu, 0x42, "WDM", addressing_mode_imm              , operation_function_WDM, cycle_count_mode_00, 2, 2, 0);
    cpu_register_op_code(myCpu, 0x43, "EOR", addressing_mode_stk_s            , operation_function_EOR, cycle_count_mode_08, 5, 2, 0);
    cpu_register_op_code(myCpu, 0x44, "MVP", addressing_mode_src_dest         , operation_function_MVP, cycle_count_mode_00, 7, 3, 0);
    cpu_register_op_code(myCpu, 0x45, "EOR", addressing_mode_dir              , operation_function_EOR, cycle_count_mode_09, 4, 2, 0);
    cpu_register_op_code(myCpu, 0x46, "LSR", addressing_mode_dir              , operation_function_LSR, cycle_count_mode_05, 7, 2, 0);
    cpu_register_op_code(myCpu, 0x47, "EOR", addressing_mode_dir_24bit        , operation_function_EOR, cycle_count_mode_09, 7, 2, 0);
    cpu_register_op_code(myCpu, 0x48, "PHA", addressing_mode_imp              , operation_function_PHA, cycle_count_mode_08, 4, 1, 0);
    cpu_register_op_code(myCpu, 0x49, "EOR", addressing_mode_imm              , operation_function_EOR, cycle_count_mode_08, 3, 3, 1);
    cpu_register_op_code(myCpu, 0x4A, "LSR", addressing_mode_acc              , operation_function_LSR, cycle_count_mode_00, 2, 1, 0);
    cpu_register_op_code(myCpu, 0x4B, "PHK", addressing_mode_imp              , operation_function_PHK, cycle_count_mode_00, 3, 1, 0);
    cpu_register_op_code(myCpu, 0x4C, "JMP", addressing_mode_abs              , operation_function_JMP, cycle_count_mode_00, 3, 3, 0);
    cpu_register_op_code(myCpu, 0x4D, "EOR", addressing_mode_abs              , operation_function_EOR, cycle_count_mode_08, 5, 3, 0);
    cpu_register_op_code(myCpu, 0x4E, "LSR", addressing_mode_abs              , operation_function_LSR, cycle_count_mode_04, 8, 3, 0);
    cpu_register_op_code(myCpu, 0x4F, "EOR", addressing_mode_long             , operation_function_EOR, cycle_count_mode_08, 6, 4, 0);
    cpu_register_op_code(myCpu, 0x50, "BVC", addressing_mode_rel_8            , operation_function_BVC, cycle_count_mode_02, 2, 2, 0);
    cpu_register_op_code(myCpu, 0x51, "EOR", addressing_mode_dir_y_16bit      , operation_function_EOR, cycle_count_mode_0A, 7, 2, 0);
    cpu_register_op_code(myCpu, 0x52, "EOR", addressing_mode_dir_16bit        , operation_function_EOR, cycle_count_mode_09, 6, 2, 0);
    cpu_register_op_code(myCpu, 0x53, "EOR", addressing_mode_stk_s_y          , operation_function_EOR, cycle_count_mode_08, 8, 2, 0);
    cpu_register_op_code(myCpu, 0x54, "MVN", addressing_mode_src_dest         , operation_function_MVN, cycle_count_mode_00, 7, 3, 0);
    cpu_register_op_code(myCpu, 0x55, "EOR", addressing_mode_dir_offset_x     , operation_function_EOR, cycle_count_mode_09, 5, 2, 0);
    cpu_register_op_code(myCpu, 0x56, "LSR", addressing_mode_dir_offset_x     , operation_function_LSR, cycle_count_mode_05, 8, 2, 0);
    cpu_register_op_code(myCpu, 0x57, "EOR", addressing_mode_dir_y_24bit      , operation_function_EOR, cycle_count_mode_09, 7, 2, 0);
    cpu_register_op_code(myCpu, 0x58, "CLI", addressing_mode_imp              , operation_function_CLI, cycle_count_mode_00, 2, 1, 0);
    cpu_register_op_code(myCpu, 0x59, "EOR", addressing_mode_abs_offset_y     , operation_function_EOR, cycle_count_mode_0B, 6, 3, 0);
    cpu_register_op_code(myCpu, 0x5A, "PHY", addressing_mode_imp              , operation_function_PHY, cycle_count_mode_0C, 4, 1, 0);
    cpu_register_op_code(myCpu, 0x5B, "TCD", addressing_mode_imp              , operation_function_TCD, cycle_count_mode_00, 2, 1, 0);
    cpu_register_op_code(myCpu, 0x5C, "JMP", addressing_mode_long             , operation_function_JMP, cycle_count_mode_00, 4, 4, 0);
    cpu_register_op_code(myCpu, 0x5D, "EOR", addressing_mode_abs_offset_x     , operation_function_EOR, cycle_count_mode_0B, 6, 3, 0);
    cpu_register_op_code(myCpu, 0x5E, "LSR", addressing_mode_abs_offset_x     , operation_function_LSR, cycle_count_mode_04, 9, 3, 0);
    cpu_register_op_code(myCpu, 0x5F, "EOR", addressing_mode_long_x           , operation_function_EOR, cycle_count_mode_08, 6, 4, 0);
    cpu_register_op_code(myCpu, 0x60, "RTS", addressing_mode_imp              , operation_function_RTS, cycle_count_mode_00, 6, 1, 0);
    cpu_register_op_code(myCpu, 0x61, "ADC", addressing_mode_dir_offset_x_bank, operation_function_ADC, cycle_count_mode_09, 7, 2, 0);
    cpu_register_op_code(myCpu, 0x62, "PER", addressing_mode_imm              , operation_function_PER, cycle_count_mode_00, 6, 3, 0);
    cpu_register_op_code(myCpu, 0x63, "ADC", addressing_mode_stk_s            , operation_function_ADC, cycle_count_mode_08, 5, 2, 0);
    cpu_register_op_code(myCpu, 0x64, "STZ", addressing_mode_dir              , operation_function_STZ, cycle_count_mode_09, 4, 2, 0);
    cpu_register_op_code(myCpu, 0x65, "ADC", addressing_mode_dir              , operation_function_ADC, cycle_count_mode_09, 4, 2, 0);
    cpu_register_op_code(myCpu, 0x66, "ROR", addressing_mode_dir              , operation_function_ROR, cycle_count_mode_05, 7, 2, 0);
    cpu_register_op_code(myCpu, 0x67, "ADC", addressing_mode_dir_24bit        , operation_function_ADC, cycle_count_mode_09, 7, 2, 0);
    cpu_register_op_code(myCpu, 0x68, "PLA", addressing_mode_imp              , operation_function_PLA, cycle_count_mode_08, 5, 1, 0);
    cpu_register_op_code(myCpu, 0x69, "ADC", addressing_mode_imm              , operation_function_ADC, cycle_count_mode_08, 3, 3, 1);
    cpu_register_op_code(myCpu, 0x6A, "ROR", addressing_mode_acc              , operation_function_ROR, cycle_count_mode_00, 2, 1, 0);
    cpu_register_op_code(myCpu, 0x6B, "RTL", addressing_mode_imp              , operation_function_RTL, cycle_count_mode_00, 6, 1, 0);
    cpu_register_op_code(myCpu, 0x6C, "JMP", addressing_mode_abs_16bit        , operation_function_JMP, cycle_count_mode_00, 5, 3, 0);
    cpu_register_op_code(myCpu, 0x6D, "ADC", addressing_mode_abs              , operation_function_ADC, cycle_count_mode_08, 5, 3, 0);
    cpu_register_op_code(myCpu, 0x6E, "ROR", addressing_mode_abs              , operation_function_ROR, cycle_count_mode_04, 8, 3, 0);
    cpu_register_op_code(myCpu, 0x6F, "ADC", addressing_mode_long             , operation_function_ADC, cycle_count_mode_08, 6, 4, 0);
    cpu_register_op_code(myCpu, 0x70, "BVS", addressing_mode_rel_8            , operation_function_BVS, cycle_count_mode_02, 2, 2, 0);
    cpu_register_op_code(myCpu, 0x71, "ADC", addressing_mode_dir_y_16bit      , operation_function_ADC, cycle_count_mode_0A, 7, 2, 0);
    cpu_register_op_code(myCpu, 0x72, "ADC", addressing_mode_dir_16bit        , operation_function_ADC, cycle_count_mode_09, 6, 2, 0);
    cpu_register_op_code(myCpu, 0x73, "ADC", addressing_mode_stk_s_y          , operation_function_ADC, cycle_count_mode_08, 8, 2, 0);
    cpu_register_op_code(myCpu, 0x74, "STZ", addressing_mode_dir_offset_x     , operation_function_STZ, cycle_count_mode_09, 5, 2, 0);
    cpu_register_op_code(myCpu, 0x75, "ADC", addressing_mode_dir_offset_x     , operation_function_ADC, cycle_count_mode_09, 5, 2, 0);
    cpu_register_op_code(myCpu, 0x76, "ROR", addressing_mode_dir_offset_x     , operation_function_ROR, cycle_count_mode_05, 8, 2, 0);
    cpu_register_op_code(myCpu, 0x77, "ADC", addressing_mode_dir_y_24bit      , operation_function_ADC, cycle_count_mode_09, 7, 2, 0);
    cpu_register_op_code(myCpu, 0x78, "SEI", addressing_mode_imp              , operation_function_SEI, cycle_count_mode_00, 2, 1, 0);
    cpu_register_op_code(myCpu, 0x79, "ADC", addressing_mode_abs_offset_y     , operation_function_ADC, cycle_count_mode_0B, 6, 3, 0);
    cpu_register_op_code(myCpu, 0x7A, "PLY", addressing_mode_imp              , operation_function_PLY, cycle_count_mode_0C, 5, 1, 0);
    cpu_register_op_code(myCpu, 0x7B, "TDC", addressing_mode_imp              , operation_function_TDC, cycle_count_mode_00, 2, 1, 0);
    cpu_register_op_code(myCpu, 0x7C, "JMP", addressing_mode_abs_offset_x_bank, operation_function_JMP, cycle_count_mode_00, 6, 3, 0);
    cpu_register_op_code(myCpu, 0x7D, "ADC", addressing_mode_abs_offset_x     , operation_function_ADC, cycle_count_mode_0B, 6, 3, 0);
    cpu_register_op_code(myCpu, 0x7E, "ROR", addressing_mode_abs_offset_x     , operation_function_ROR, cycle_count_mode_04, 9, 3, 0);
    cpu_register_op_code(myCpu, 0x7F, "ADC", addressing_mode_long_x           , operation_function_ADC, cycle_count_mode_08, 6, 4, 0);
    cpu_register_op_code(myCpu, 0x80, "BRA", addressing_mode_rel_8            , operation_function_BRA, cycle_count_mode_01, 3, 2, 0);
    cpu_register_op_code(myCpu, 0x81, "STA", addressing_mode_dir_offset_x_bank, operation_function_STA, cycle_count_mode_09, 7, 2, 0);
    cpu_register_op_code(myCpu, 0x82, "BRL", addressing_mode_rel_16           , operation_function_BRL, cycle_count_mode_00, 4, 3, 0);
    cpu_register_op_code(myCpu, 0x83, "STA", addressing_mode_stk_s            , operation_function_STA, cycle_count_mode_08, 5, 2, 0);
    cpu_register_op_code(myCpu, 0x84, "STY", addressing_mode_dir              , operation_function_STY, cycle_count_mode_0D, 4, 2, 0);
    cpu_register_op_code(myCpu, 0x85, "STA", addressing_mode_dir              , operation_function_STA, cycle_count_mode_09, 4, 2, 0);
    cpu_register_op_code(myCpu, 0x86, "STX", addressing_mode_dir              , operation_function_STX, cycle_count_mode_0D, 4, 2, 0);
    cpu_register_op_code(myCpu, 0x87, "STA", addressing_mode_dir_24bit        , operation_function_STA, cycle_count_mode_09, 7, 2, 0);
    cpu_register_op_code(myCpu, 0x88, "DEY", addressing_mode_imp              , operation_function_DEY, cycle_count_mode_00, 2, 1, 0);
    cpu_register_op_code(myCpu, 0x89, "BIT", addressing_mode_imm              , operation_function_BIT, cycle_count_mode_08, 3, 3, 1);
    cpu_register_op_code(myCpu, 0x8A, "TXA", addressing_mode_imp              , operation_function_TXA, cycle_count_mode_00, 2, 1, 0);
    cpu_register_op_code(myCpu, 0x8B, "PHB", addressing_mode_imp              , operation_function_PHB, cycle_count_mode_00, 3, 1, 0);
    cpu_register_op_code(myCpu, 0x8C, "STY", addressing_mode_abs              , operation_function_STY, cycle_count_mode_0C, 5, 3, 0);
    cpu_register_op_code(myCpu, 0x8D, "STA", addressing_mode_abs              , operation_function_STA, cycle_count_mode_08, 5, 3, 0);
    cpu_register_op_code(myCpu, 0x8E, "STX", addressing_mode_abs              , operation_function_STX, cycle_count_mode_0C, 5, 3, 0);
    cpu_register_op_code(myCpu, 0x8F, "STA", addressing_mode_long             , operation_function_STA, cycle_count_mode_08, 6, 4, 0);
    cpu_register_op_code(myCpu, 0x90, "BCC", addressing_mode_rel_8            , operation_function_BCC, cycle_count_mode_02, 2, 2, 0);
    cpu_register_op_code(myCpu, 0x91, "STA", addressing_mode_dir_y_16bit      , operation_function_STA, cycle_count_mode_09, 7, 2, 0);
    cpu_register_op_code(myCpu, 0x92, "STA", addressing_mode_dir_16bit        , operation_function_STA, cycle_count_mode_09, 6, 2, 0);
    cpu_register_op_code(myCpu, 0x93, "STA", addressing_mode_stk_s_y          , operation_function_STA, cycle_count_mode_08, 8, 2, 0);
    cpu_register_op_code(myCpu, 0x94, "STY", addressing_mode_dir_offset_x     , operation_function_STY, cycle_count_mode_0D, 5, 2, 0);
    cpu_register_op_code(myCpu, 0x95, "STA", addressing_mode_dir_offset_x     , operation_function_STA, cycle_count_mode_09, 5, 2, 0);
    cpu_register_op_code(myCpu, 0x96, "STX", addressing_mode_dir_offset_y     , operation_function_STX, cycle_count_mode_0D, 5, 2, 0);
    cpu_register_op_code(myCpu, 0x97, "STA", addressing_mode_dir_y_24bit      , operation_function_STA, cycle_count_mode_09, 7, 2, 0);
    cpu_register_op_code(myCpu, 0x98, "TYA", addressing_mode_imp              , operation_function_TYA, cycle_count_mode_00, 2, 1, 0);
    cpu_register_op_code(myCpu, 0x99, "STA", addressing_mode_abs_offset_y     , operation_function_STA, cycle_count_mode_08, 6, 3, 0);
    cpu_register_op_code(myCpu, 0x9A, "TXS", addressing_mode_imp              , operation_function_TXS, cycle_count_mode_00, 2, 1, 0);
    cpu_register_op_code(myCpu, 0x9B, "TXY", addressing_mode_imp              , operation_function_TXY, cycle_count_mode_00, 2, 1, 0);
    cpu_register_op_code(myCpu, 0x9C, "STZ", addressing_mode_abs              , operation_function_STZ, cycle_count_mode_08, 5, 3, 0);
    cpu_register_op_code(myCpu, 0x9D, "STA", addressing_mode_abs_offset_x     , operation_function_STA, cycle_count_mode_08, 6, 3, 0);
    cpu_register_op_code(myCpu, 0x9E, "STZ", addressing_mode_abs_offset_x     , operation_function_STZ, cycle_count_mode_08, 6, 3, 0);
    cpu_register_op_code(myCpu, 0x9F, "STA", addressing_mode_long_x           , operation_function_STA, cycle_count_mode_08, 6, 4, 0);
    cpu_register_op_code(myCpu, 0xA0, "LDY", addressing_mode_imm              , operation_function_LDY, cycle_count_mode_0C, 3, 3, 2);
    cpu_register_op_code(myCpu, 0xA1, "LDA", addressing_mode_dir_offset_x_bank, operation_function_LDA, cycle_count_mode_09, 7, 2, 0);
    cpu_register_op_code(myCpu, 0xA2, "LDX", addressing_mode_imm              , operation_function_LDX, cycle_count_mode_0C, 3, 3, 2);
    cpu_register_op_code(myCpu, 0xA3, "LDA", addressing_mode_stk_s            , operation_function_LDA, cycle_count_mode_08, 5, 2, 0);
    cpu_register_op_code(myCpu, 0xA4, "LDY", addressing_mode_dir              , operation_function_LDY, cycle_count_mode_0D, 4, 2, 0);
    cpu_register_op_code(myCpu, 0xA5, "LDA", addressing_mode_dir              , operation_function_LDA, cycle_count_mode_09, 4, 2, 0);
    cpu_register_op_code(myCpu, 0xA6, "LDX", addressing_mode_dir              , operation_function_LDX, cycle_count_mode_0D, 4, 2, 0);
    cpu_register_op_code(myCpu, 0xA7, "LDA", addressing_mode_dir_24bit        , operation_function_LDA, cycle_count_mode_09, 7, 2, 0);
    cpu_register_op_code(myCpu, 0xA8, "TAY", addressing_mode_imp              , operation_function_TAY, cycle_count_mode_00, 2, 1, 0);
    cpu_register_op_code(myCpu, 0xA9, "LDA", addressing_mode_imm              , operation_function_LDA, cycle_count_mode_08, 3, 3, 1);
    cpu_register_op_code(myCpu, 0xAA, "TAX", addressing_mode_imp              , operation_function_TAX, cycle_count_mode_00, 2, 1, 0);
    cpu_register_op_code(myCpu, 0xAB, "PLB", addressing_mode_imp              , operation_function_PLB, cycle_count_mode_00, 4, 1, 0);
    cpu_register_op_code(myCpu, 0xAC, "LDY", addressing_mode_abs              , operation_function_LDY, cycle_count_mode_0C, 5, 3, 0);
    cpu_register_op_code(myCpu, 0xAD, "LDA", addressing_mode_abs              , operation_function_LDA, cycle_count_mode_08, 5, 3, 0);
    cpu_register_op_code(myCpu, 0xAE, "LDX", addressing_mode_abs              , operation_function_LDX, cycle_count_mode_0C, 5, 3, 0);
    cpu_register_op_code(myCpu, 0xAF, "LDA", addressing_mode_long             , operation_function_LDA, cycle_count_mode_08, 6, 4, 0);
    cpu_register_op_code(myCpu, 0xB0, "BCS", addressing_mode_rel_8            , operation_function_BCS, cycle_count_mode_02, 2, 2, 0);
    cpu_register_op_code(myCpu, 0xB1, "LDA", addressing_mode_dir_y_16bit      , operation_function_LDA, cycle_count_mode_0A, 7, 2, 0);
    cpu_register_op_code(myCpu, 0xB2, "LDA", addressing_mode_dir_16bit        , operation_function_LDA, cycle_count_mode_09, 6, 2, 0);
    cpu_register_op_code(myCpu, 0xB3, "LDA", addressing_mode_stk_s_y          , operation_function_LDA, cycle_count_mode_08, 8, 2, 0);
    cpu_register_op_code(myCpu, 0xB4, "LDY", addressing_mode_dir_offset_x     , operation_function_LDY, cycle_count_mode_0D, 5, 2, 0);
    cpu_register_op_code(myCpu, 0xB5, "LDA", addressing_mode_dir_offset_x     , operation_function_LDA, cycle_count_mode_09, 5, 2, 0);
    cpu_register_op_code(myCpu, 0xB6, "LDX", addressing_mode_dir_offset_y     , operation_function_LDX, cycle_count_mode_0D, 5, 2, 0);
    cpu_register_op_code(myCpu, 0xB7, "LDA", addressing_mode_dir_y_24bit      , operation_function_LDA, cycle_count_mode_09, 7, 2, 0);
    cpu_register_op_code(myCpu, 0xB8, "CLV", addressing_mode_imp              , operation_function_CLV, cycle_count_mode_00, 2, 1, 0);
    cpu_register_op_code(myCpu, 0xB9, "LDA", addressing_mode_abs_offset_y     , operation_function_LDA, cycle_count_mode_0B, 6, 3, 0);
    cpu_register_op_code(myCpu, 0xBA, "TSX", addressing_mode_imp              , operation_function_TSX, cycle_count_mode_00, 2, 1, 0);
    cpu_register_op_code(myCpu, 0xBB, "TYX", addressing_mode_imp              , operation_function_TYX, cycle_count_mode_00, 2, 1, 0);
    cpu_register_op_code(myCpu, 0xBC, "LDY", addressing_mode_abs_offset_x     , operation_function_LDY, cycle_count_mode_06, 6, 3, 0);
    cpu_register_op_code(myCpu, 0xBD, "LDA", addressing_mode_abs_offset_x     , operation_function_LDA, cycle_count_mode_0B, 6, 3, 0);
    cpu_register_op_code(myCpu, 0xBE, "LDX", addressing_mode_abs_offset_y     , operation_function_LDX, cycle_count_mode_06, 6, 3, 0);
    cpu_register_op_code(myCpu, 0xBF, "LDA", addressing_mode_long_x           , operation_function_LDA, cycle_count_mode_08, 6, 4, 0);
    cpu_register_op_code(myCpu, 0xC0, "CPY", addressing_mode_imm              , operation_function_CPY, cycle_count_mode_0C, 3, 3, 2);
    cpu_register_op_code(myCpu, 0xC1, "CMP", addressing_mode_dir_offset_x_bank, operation_function_CMP, cycle_count_mode_09, 7, 2, 0);
    cpu_register_op_code(myCpu, 0xC2, "REP", addressing_mode_imm              , operation_function_REP, cycle_count_mode_00, 3, 2, 0);
    cpu_register_op_code(myCpu, 0xC3, "CMP", addressing_mode_stk_s            , operation_function_CMP, cycle_count_mode_08, 5, 2, 0);
    cpu_register_op_code(myCpu, 0xC4, "CPY", addressing_mode_dir              , operation_function_CPY, cycle_count_mode_0D, 4, 2, 0);
    cpu_register_op_code(myCpu, 0xC5, "CMP", addressing_mode_dir              , operation_function_CMP, cycle_count_mode_09, 4, 2, 0);
    cpu_register_op_code(myCpu, 0xC6, "DEC", addressing_mode_dir              , operation_function_DEC, cycle_count_mode_05, 7, 2, 0);
    cpu_register_op_code(myCpu, 0xC7, "CMP", addressing_mode_dir_24bit        , operation_function_CMP, cycle_count_mode_09, 7, 2, 0);
    cpu_register_op_code(myCpu, 0xC8, "INY", addressing_mode_imp              , operation_function_INY, cycle_count_mode_00, 2, 1, 0);
    cpu_register_op_code(myCpu, 0xC9, "CMP", addressing_mode_imm              , operation_function_CMP, cycle_count_mode_08, 3, 3, 1);
    cpu_register_op_code(myCpu, 0xCA, "DEX", addressing_mode_imp              , operation_function_DEX, cycle_count_mode_00, 2, 1, 0);
    cpu_register_op_code(myCpu, 0xCB, "WAI", addressing_mode_imp              , operation_function_WAI, cycle_count_mode_00, 3, 1, 0);
    cpu_register_op_code(myCpu, 0xCC, "CPY", addressing_mode_abs              , operation_function_CPY, cycle_count_mode_0C, 5, 3, 0);
    cpu_register_op_code(myCpu, 0xCD, "CMP", addressing_mode_abs              , operation_function_CMP, cycle_count_mode_08, 5, 3, 0);
    cpu_register_op_code(myCpu, 0xCE, "DEC", addressing_mode_abs              , operation_function_DEC, cycle_count_mode_04, 8, 3, 0);
    cpu_register_op_code(myCpu, 0xCF, "CMP", addressing_mode_long             , operation_function_CMP, cycle_count_mode_08, 6, 4, 0);
    cpu_register_op_code(myCpu, 0xD0, "BNE", addressing_mode_rel_8            , operation_function_BNE, cycle_count_mode_02, 2, 2, 0);
    cpu_register_op_code(myCpu, 0xD1, "CMP", addressing_mode_dir_y_16bit      , operation_function_CMP, cycle_count_mode_0A, 7, 2, 0);
    cpu_register_op_code(myCpu, 0xD2, "CMP", addressing_mode_dir_16bit        , operation_function_CMP, cycle_count_mode_09, 6, 2, 0);
    cpu_register_op_code(myCpu, 0xD3, "CMP", addressing_mode_stk_s_y          , operation_function_CMP, cycle_count_mode_08, 8, 2, 0);
    cpu_register_op_code(myCpu, 0xD4, "PEI", addressing_mode_dir              , operation_function_PEI, cycle_count_mode_03, 6, 2, 0);
    cpu_register_op_code(myCpu, 0xD5, "CMP", addressing_mode_dir_offset_x     , operation_function_CMP, cycle_count_mode_09, 5, 2, 0);
    cpu_register_op_code(myCpu, 0xD6, "DEC", addressing_mode_dir_offset_x     , operation_function_DEC, cycle_count_mode_05, 8, 2, 0);
    cpu_register_op_code(myCpu, 0xD7, "CMP", addressing_mode_dir_y_24bit      , operation_function_CMP, cycle_count_mode_09, 7, 2, 0);
    cpu_register_op_code(myCpu, 0xD8, "CLD", addressing_mode_imp              , operation_function_CLD, cycle_count_mode_00, 2, 1, 0);
    cpu_register_op_code(myCpu, 0xD9, "CMP", addressing_mode_abs_offset_y     , operation_function_CMP, cycle_count_mode_0B, 6, 3, 0);
    cpu_register_op_code(myCpu, 0xDA, "PHX", addressing_mode_imp              , operation_function_PHX, cycle_count_mode_0C, 4, 1, 0);
    cpu_register_op_code(myCpu, 0xDB, "STP", addressing_mode_imp              , operation_function_STP, cycle_count_mode_00, 3, 1, 0);
    cpu_register_op_code(myCpu, 0xDC, "JMP", addressing_mode_abs_24bit        , operation_function_JMP, cycle_count_mode_00, 6, 3, 0);
    cpu_register_op_code(myCpu, 0xDD, "CMP", addressing_mode_abs_offset_x     , operation_function_CMP, cycle_count_mode_0B, 6, 3, 0);
    cpu_register_op_code(myCpu, 0xDE, "DEC", addressing_mode_abs_offset_x     , operation_function_DEC, cycle_count_mode_04, 9, 3, 0);
    cpu_register_op_code(myCpu, 0xDF, "CMP", addressing_mode_long_x           , operation_function_CMP, cycle_count_mode_08, 6, 4, 0);
    cpu_register_op_code(myCpu, 0xE0, "CPX", addressing_mode_imm              , operation_function_CPX, cycle_count_mode_0C, 3, 3, 2);
    cpu_register_op_code(myCpu, 0xE1, "SBC", addressing_mode_dir_offset_x_bank, operation_function_SBC, cycle_count_mode_09, 7, 2, 0);
    cpu_register_op_code(myCpu, 0xE2, "SEP", addressing_mode_imm              , operation_function_SEP, cycle_count_mode_00, 3, 2, 0);
    cpu_register_op_code(myCpu, 0xE3, "SBC", addressing_mode_stk_s            , operation_function_SBC, cycle_count_mode_08, 5, 2, 0);
    cpu_register_op_code(myCpu, 0xE4, "CPX", addressing_mode_dir              , operation_function_CPX, cycle_count_mode_0D, 4, 2, 0);
    cpu_register_op_code(myCpu, 0xE5, "SBC", addressing_mode_dir              , operation_function_SBC, cycle_count_mode_09, 4, 2, 0);
    cpu_register_op_code(myCpu, 0xE6, "INC", addressing_mode_dir              , operation_function_INC, cycle_count_mode_05, 7, 2, 0);
    cpu_register_op_code(myCpu, 0xE7, "SBC", addressing_mode_dir_24bit        , operation_function_SBC, cycle_count_mode_09, 7, 2, 0);
    cpu_register_op_code(myCpu, 0xE8, "INX", addressing_mode_imp              , operation_function_INX, cycle_count_mode_00, 2, 1, 0);
    cpu_register_op_code(myCpu, 0xE9, "SBC", addressing_mode_imm              , operation_function_SBC, cycle_count_mode_08, 3, 3, 1);
    cpu_register_op_code(myCpu, 0xEA, "NOP", addressing_mode_imp              , operation_function_NOP, cycle_count_mode_00, 2, 1, 0);
    cpu_register_op_code(myCpu, 0xEB, "XBA", addressing_mode_imp              , operation_function_XBA, cycle_count_mode_00, 3, 1, 0);
    cpu_register_op_code(myCpu, 0xEC, "CPX", addressing_mode_abs              , operation_function_CPX, cycle_count_mode_0C, 5, 3, 0);
    cpu_register_op_code(myCpu, 0xED, "SBC", addressing_mode_abs              , operation_function_SBC, cycle_count_mode_08, 5, 3, 0);
    cpu_register_op_code(myCpu, 0xEE, "INC", addressing_mode_abs              , operation_function_INC, cycle_count_mode_04, 8, 3, 0);
    cpu_register_op_code(myCpu, 0xEF, "SBC", addressing_mode_long             , operation_function_SBC, cycle_count_mode_08, 6, 4, 0);
    cpu_register_op_code(myCpu, 0xF0, "BEQ", addressing_mode_rel_8            , operation_function_BEQ, cycle_count_mode_02, 2, 2, 0);
    cpu_register_op_code(myCpu, 0xF1, "SBC", addressing_mode_dir_y_16bit      , operation_function_SBC, cycle_count_mode_0A, 7, 2, 0);
    cpu_register_op_code(myCpu, 0xF2, "SBC", addressing_mode_dir_16bit        , operation_function_SBC, cycle_count_mode_09, 6, 2, 0);
    cpu_register_op_code(myCpu, 0xF3, "SBC", addressing_mode_stk_s_y          , operation_function_SBC, cycle_count_mode_08, 8, 2, 0);
    cpu_register_op_code(myCpu, 0xF4, "PEA", addressing_mode_imm              , operation_function_PEA, cycle_count_mode_00, 5, 3, 0);
    cpu_register_op_code(myCpu, 0xF5, "SBC", addressing_mode_dir_offset_x     , operation_function_SBC, cycle_count_mode_09, 5, 2, 0);
    cpu_register_op_code(myCpu, 0xF6, "INC", addressing_mode_dir_offset_x     , operation_function_INC, cycle_count_mode_05, 8, 2, 0);
    cpu_register_op_code(myCpu, 0xF7, "SBC", addressing_mode_dir_y_24bit      , operation_function_SBC, cycle_count_mode_09, 7, 2, 0);
    cpu_register_op_code(myCpu, 0xF8, "SED", addressing_mode_imp              , operation_function_SED, cycle_count_mode_00, 2, 1, 0);
    cpu_register_op_code(myCpu, 0xF9, "SBC", addressing_mode_abs_offset_y     , operation_function_SBC, cycle_count_mode_0B, 6, 3, 0);
    cpu_register_op_code(myCpu, 0xFA, "PLX", addressing_mode_imp              , operation_function_PLX, cycle_count_mode_0C, 5, 1, 0);
    cpu_register_op_code(myCpu, 0xFB, "XCE", addressing_mode_imp              , operation_function_XCE, cycle_count_mode_00, 2, 1, 0);
    cpu_register_op_code(myCpu, 0xFC, "JSR", addressing_mode_abs_offset_x_bank, operation_function_JSR, cycle_count_mode_00, 8, 3, 0);
    cpu_register_op_code(myCpu, 0xFD, "SBC", addressing_mode_abs_offset_x     , operation_function_SBC, cycle_count_mode_0B, 6, 3, 0);
    cpu_register_op_code(myCpu, 0xFE, "INC", addressing_mode_abs_offset_x     , operation_function_INC, cycle_count_mode_04, 9, 3, 0);
    cpu_register_op_code(myCpu, 0xFF, "SBC", addressing_mode_long_x           , operation_function_SBC, cycle_count_mode_08, 6, 4, 0);
}

#pragma endregion
