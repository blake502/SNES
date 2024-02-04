#pragma once
#include "defines.h"

/*
    Built from information found here:
    http://www.6502.org/tutorials/65c816opcodes.html

    Section 4 implemented.
    Section 5 in progress.
*/

typedef struct cpu cpu;

void cpu_initialize(cpu* myCpu);
void cpu_reset(cpu* myCpu);

void cpu_load_ROM(cpu* myCpu);
void cpu_clock(cpu* myCpu);

u16 cpu_read_16bit(cpu* myCpu, u32 address);
u8 cpu_read_8bit(cpu* myCpu, u32 address);
void cpu_write_16bit(cpu* myCpu, u32 address, u16 data);
void cpu_write_8bit(cpu* myCpu, u32 address, u8 data);

void cpu_register_all_op_codes(cpu* myCpu);

typedef struct op_code {
    //A name for the op code
    const char* instruction_name;
    //The addressing mode used for the op code
    u8 (*addressing_mode)(cpu* myCpu);
    //The operation for the op code
    u8 (*operation_function_pointer)(cpu* myCpu);
    //The cycles calculation for the op code
    u8 (*operation_cycles)(cpu* myCpu, u8 absolute, u8 page_boundary_crossed, u8 branch_taken);
    //The base number of cycles
    u8 cycles;

    //The base length
    u8 length;
    //The length calculation mode (for IMM with x or m)
    u8 length_mode;
} op_code;

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
    
    u8 current_op_code;
    u32 current_op_code_pc;

    union
    {
        u32 fetched_data;
        struct
        {
            u8 fetched_data_lo;
            u8 fetched_data_hi;
            u8 fetched_data_bank;
        };
        
    };

    union
    {
        u32 destination;
        struct
        {
            u8 destination_lo;
            u8 destination_hi;
            u8 destination_bank;
        };
    };

    u8 cycles;

} cpu;

