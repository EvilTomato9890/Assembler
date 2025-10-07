#ifndef PROCESSOR_DATA_H_INCLUDED
#define PROCESSOR_DATA_H_INCLUDED

#include "stack.h"

enum command_num {
	cmdPUSH = 1,
	cmdADD  = 2,
	cmdSUB  = 3,
	cmdDEL  = 4,
	cmdMULT = 5,
	cmdPOW  = 6,
	cmdSQRT = 7,
	cmdOUT  = 8,
	cmdHLT  = 9
};

struct processor_data_t {
	stack_t* stack;
	st_type* registers;
	char* num_commands;
};

error_code processor_data_init(processor_data_t* data, const char* file_name);
error_code processor_data_dest(processor_data_t* data);



#endif