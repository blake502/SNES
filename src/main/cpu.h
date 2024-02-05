#pragma once
#include "defines.h"

/*
    Built from information found here:
    http://www.6502.org/tutorials/65c816opcodes.html

    Section 4 implemented.
    Section 5 in progress.
*/

//Forward declaring cpu type
typedef struct cpu cpu;

//TODO: REMOVE
void cpu_load_SMW(cpu* myCpu);

//Called once to prepare the CPU and register op codes
void cpu_initialize(cpu* myCpu);
//Called to reset the CPU to initial state
void cpu_reset(cpu* myCpu);

//Called to load ROM (from file?)
void cpu_load_ROM(cpu* myCpu);
//Called in a loop to clock the CPU
void cpu_clock(cpu* myCpu);

//TODO:
//Implement functions for NMI, IRQ, etc

//Reads two 8-bit values at a given address and returns them as a 16-bit value
u16 cpu_read_16bit(cpu* myCpu, u32 address);
//Reads 8-bit value at a given address
u8 cpu_read_8bit(cpu* myCpu, u32 address);
//Writes two 8-bit values at a given address from a given 16-bit value
void cpu_write_16bit(cpu* myCpu, u32 address, u16 data);
//Writes 8-bit value at a given address
void cpu_write_8bit(cpu* myCpu, u32 address, u8 data);

//Called once by cpu_initialize to register op codes
void cpu_register_all_op_codes(cpu* myCpu);

//TODO: This should not be in the header
u8 cpu_get_instruction_length(cpu* myCpu, u8 base_length, u8 length_mode);

//Op code
//Op codes contain several things:
//A name for the operation EX: "JMP"
//An addressing mode function EX: abs
//An operation function EX: JMP
//A base number of cycles the instructions takes
//A calculation function for the actual number of cycles
//A base length of the instruction
//A calculation function for the actual length
typedef struct op_code {
    //A name for the op code
    const char* instruction_name;

    //The addressing mode used for the op code
    //The addressing mode function should return 1 if a page boundary is crossed.
    u8 (*addressing_mode_func_ptr)(cpu* myCpu);

    //The operation for the op code
    //The operation function should return 1 if a branch is taken
    u8 (*operation_func_ptr)(cpu* myCpu);

    //The base number of cycles
    u8 base_cycles;
    //The cycles calculation mode for the op code
    u8 (*cycles_calculation_func_ptr)(cpu* myCpu, u8 absolute, u8 page_boundary_crossed, u8 branch_taken);

    //The base length
    u8 base_length;
    //The length calculation mode (this is exclusively for some IMM addressed operations, and only if flag_x or flag_m is 1)
    //Note, there is no function pointer for each length_mode because there are only 3 modes
    //They are handled by a switch/case statement instead in cpu_get_instruction_length.
    u8 length_mode;
} op_code;

//The cpu struct holds everything related to the CPU's current state
//Some things here may be refactored later to better represent the CPU's state
typedef struct cpu {
    //Accumulator
    union {
        /*
            C accumulator (16-bit)
            This should be considered the "accumulator" if flag_accumulator_width is 0 (flag m)
        */
        u16 accumulator_c;

        struct {
            /*
                A accumulator (8-bit)
                Accumulator low byte
                This should be considered the "accumulator" if flag_accumulator_width is 1 (flag m)
            */
            u8 accumulator_a;
            /*
                B accumulator (8-bit)
                Accumulator high byte
            */
            u8 accumulator_b;
        };
    };

    //Direct register
    union {
        /*
            D (16-bit)
            Direct register
        */
        u16 register_direct;

        struct {
            /*
                DL (8-bit)
                Direct register low byte
            */
            u8 register_dl;

            /*
                DH (8-bit)
                Direct register hight byte
            */
            u8 register_dh;
        };
    };

    //Program counter
    union {
        /*
            PC (16-bit)
            Program counter
        */
        u16 program_counter;

        struct {
            /*
                PCL (8-bit)
                Program counter low byte
            */
            u8 program_counter_l;
            
            /*
                PCH (8-bit)
                Program counter high byte
            */
            u8 program_counter_h;
        };
    };

    //Processor status flags
    union {
        /*
            P (8-bit)
            Processor status flags (bitfield)
        */
        u8 register_processor_status;

        //Bitfield
        struct {
            /*
                n flag (1-bit)
                Negative flag
                1 when the result is negative.
            */
            u8 flag_negative : 1;

            /*
                v flag (1-bit)
                Overflow flag
                1 when an arithmetic overflow occurs.
            */
            u8 flag_overflow : 1;

            /*
                m flag (1-bit)
                Accumulator width flag
                Forced to 1 when the e flag is 1.
                Accumulator and memory width flag
                When the m flag is 0, the accumulator (and memory) width is 16 bits -- Otherwise they're 8 bits.
            */
            u8 flag_accumulator_width : 1;

            /*
                x flag (1-bit)
                Index width flag
                Forced to 1 when the e flag is 1.
                When the x flag is 0, the index register width is 16 bits -- Otherwise it's 8 bits.
                When the x flag is 1 (8-bit index registers), the XH register and the YH register are both forced to 0x00.
            */
            u8 flag_index_width : 1;

            /*
                d flag (1-bit)
                Decimal mode flag
                When the d flag is 0, the ADC and SBC instructions perform binary arithmetic.
                When the d flag is 1, the ADC and SBC instructions perform BCD arithmetic. 
            */
            u8 flag_decimal_mode : 1;

            /*
                i flag (1-bit)
                Interrupt disable flag
                When the i flag is 0, (IRQ) interrupts are enabled.
                When the i flag is 1, (IRQ) interrupts are disabled.
            */
            u8 flag_interrupt_disable : 1;

            /*
                z flag (1-bit)
                Zero flag
                The z flag is 1 when the result is zero.
                The z flag is 0 when the result is non-zero.
            */
            u8 flag_zero : 1;

            /*
                c flag (1-bit)
                Carry flag
                The c flag is 1 when there is an arithmetic carry.
                The c flag is 0 when where was not an arithmetic carry.
            */
            u8 flag_carry : 1;
        };

        //Bitfield aliases
        struct
        {
            u8 flag_n : 1;
            u8 flag_v : 1;
            u8 flag_m : 1;
            u8 flag_x : 1;
            u8 flag_d : 1;
            u8 flag_i : 1;
            u8 flag_z : 1;
            u8 flag_c : 1;
        };
        

        //Bitfield aliases
        struct 
        {
            //DO NOT USE-- This is a dummy offset, not to be used. It is a 2-bit value that maps to n flag and v flag.
            u8 DUMMY_OFFSET_DO_NOT_USE : 2;
            /*
                m flag alias (1-bit)
                if flag_emulation_mode is 0 (e flag)
                When the m flag is 0, the accumulator (and memory) width is 16 bits -- Otherwise they're 8 bits.
                Accumulator and memory width flag
            */
            u8 flag_memory_width : 1;

            /*
                b flag (1-bit)
                if flag_emulation_mode is 1 (e flag)
                Used for distinguishing an IRQ (hardware) interrupt from a BRK (software) interrupt.
            */
            u8 flag_break : 1;
        };
    };
    
    /*
        e flag
        Emulation mode flag
        When the e flag is 0, the 65C816 is in native mode.
        When the e flag is 1, the 65C816 is in emulation mode.

        When the e flag is 1, the SH register is forced to $01,
        the m flag is forced to 1, and the x flag is forced to 1.
        As a consequence of the x flag being forced to 1,
        the XH register and the YH register are forced to $00

        Attempting to change the value of the SH register,
        the XH register, the YH register, the m flag,
        or the x flag when the e flag is 1 will have no effect.
    */
   union
   {
        u8 flag_emulation_mode : 1;
        u8 flag_e : 1;
   };
   

    //Stack pointer
    union {
        /*
            S (16-bit)
            Stack pointer
        */
        u16 stack_pointer;

        struct {
            /*
                SL (8-bit)
                Stack pointer low byte
            */
            u8 stack_pointer_l;

            /*
                SH (8-bit)
                Stack pointer high byte
                Forced to 0x01 when e flag is 1.
            */
            u8 stack_pointer_h;
        };
    };

    //X register
    union {
        /*
            X (16-bit)
            X register
            This should be considered the "x" register if flag_index_width is 0 (x flag)
        */
        u16 register_x;

        struct {
            /* 
                XL (8-bit)
                X register low byte
                This should be considered the "x" register if flag_index_width is 1 (x flag)
            */
            u8 register_x_l;
            /*
                XH (8-bit)
                X register high byte
            */
            u8 register_x_h;
        };
    };

    //Y register
    union {
        /*
            Y (16-bit)
            Y register
            This should be considered the "y" register if flag_index_width is 0
        */
        u16 register_y;

        struct {
            /* 
                YL (8-bit)
                Y register low byte
                This should be considered the "y" register if flag_index_width is 1
            */
            u8 register_y_l;
            /* 
                YH (8-bit)
                Y register high byte
            */
            u8 register_y_h;
        };
    };

    /*
        K (8-bit)
        Program bank register
    */
    u8 register_program_bank;

    /*
        DBR (16-bit)
        Data bank register
    */
    u16 register_data_bank;

    //TODO: Better RAM/bus
    u8 ram[16 * 1024 * 1024];

    //Holds all the op codes
    op_code op_codes[256];
    
    //Holds the program counter's state when the current op code was read.
    //This is mostly for debugging/disassembly purposes.
    u32 current_op_code_pc;
    
    
    union
    {
        //The full instruction including the entire operand
        //Instrucions can be up to 32-bits in length
        //The 8-bit op code, plus up to a 24-bit operand
        u32 current_instruction_full;
        struct 
        {
            //The current op code
            u8 current_op_code;
            //The low byte of the operand
            u8 current_LL;
            //The high byte of the operand
            //Note: The 65c816 op code doc refers to this as MM for 32-bit instructions.
            u8 current_HH;
            //The bank byte of the operand
            //Note: The 65c816 op code doc refers to this as HH for 32-bit instructions.
            u8 current_BB;
        };
    };
    
    
    union
    {
        //The entire fetched data after and addressing mode function is complete
        u32 fetched_data;
        struct
        {
            //The low byte of fetched data
            u8 fetched_data_lo;
            //The high byte of fetched data
            //Note: This is sometimes considered the middle byte
            u8 fetched_data_hi;
            //The low byte of fetched data
            //Note: This is sometimes considered the high byte
            u8 fetched_data_bank;
        };
        
    };

    union
    {
        //The entire fetched destination after and addressing mode function is complete
        u32 destination;
        struct
        {
            //The low byte of fetched destination
            u8 destination_lo;
            //The high byte of fetched destination
            //Note: This is sometimes considered the middle byte
            u8 destination_hi;
            //The high byte of fetched destination
            //Note: This is sometimes considered the middle byte
            u8 destination_bank;
        };
    };

    //The amount of cycles left to execute the current instruction
    u8 cycles;
} cpu;

