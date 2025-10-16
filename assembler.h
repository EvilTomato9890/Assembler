#ifndef ASSEMBLER_H_INCLUDED
#define ASSEMBLER_H_INCLUDED

#include "processor_data.h"

struct command_type {
	const char* cmd_name;
	command_num cmd;
};

struct command_info {
	command_num cmd;
	char* args;
};

#define COMMAND_TYPES_NUM 16

void test(char* file_buff);
error_code assemble(FILE* input_file, FILE* command_file);

#endif