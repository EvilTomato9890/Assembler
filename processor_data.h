#ifndef PROCESSOR_DATA_H_INCLUDED
#define PROCESSOR_DATA_H_INCLUDED

#include "stack.h"

//==============================================================================

#define REGISTERS_NUM 12

//==============================================================================

enum command_num {
	cmdPUSH  = 1,
	cmdADD   = 2,
	cmdSUB   = 3,
	cmdDEL   = 4,
	cmdMULT  = 5,
	cmdPOW   = 6,
	cmdSQRT  = 7,
	cmdOUT   = 8,
	cmdPOPR  = 9,
	cmdPUSHR = 10,
	cmdHLT   = 11,
	cmdJE    = 12,
	cmdJA    = 13,
	cmdJB    = 14,
	cmdJUMP  = 15,
	cmdIN    = 16
};

struct processor_data_t {
	stack_t* stack;
	st_type registers[REGISTERS_NUM];
	char* num_commands;
};

//==============================================================================

error_code processor_data_init(processor_data_t* data, const char* file_name, stack_t* stack);
error_code processor_data_dest(processor_data_t* data);

//==============================================================================

#endif