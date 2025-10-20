#ifndef PROCESSOR_DATA_H_INCLUDED
#define PROCESSOR_DATA_H_INCLUDED

#include "common/stack.h"

//==============================================================================

#define REGISTERS_NUM 12

//==============================================================================

struct processor_data_t {
	stack_t* stack;
	st_type registers[REGISTERS_NUM];
	char* commands_buff;
	size_t commands_num;
};

//==============================================================================

error_code processor_data_init(processor_data_t* data, const char* file_name, stack_t* stack);
error_code processor_data_dest(processor_data_t* data);

//==============================================================================

#endif