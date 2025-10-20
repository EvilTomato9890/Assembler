#include <stdio.h>
#include <string.h>

#include "common/stack.h"
#include "common/logger.h"
#include "common/asserts.h"
#include "common/input.h"
#include "compiler/assembler.h"
#include "cpu/processor.h"
#include "cpu/processor_data.h"

int main() {
	logger_initialize_stream(nullptr);
	error_code error = 0;
	char* file_name = 0;
	FILE* input_file = open_file(&file_name);
	if(input_file == nullptr) {
		printf("=(\n");
		return -1;
	}

	FILE* command_file = fopen("num_commands", "w");
	error |= assemble(input_file, command_file);
	RETURN_IF_ERROR((int)error, fclose(command_file););
	fclose(command_file);
	fclose(input_file);

	FILE* output_file = fopen("output", "w");
	processor_data_t processor_data = {};
	stack_t stack = {};
	processor_data_init(&processor_data, "num_commands", &stack);
	error |= execution(output_file, &processor_data);
	error |= processor_data_dest(&processor_data);
	RETURN_IF_ERROR((int)error);
	fclose(output_file);
	return 0;
}

//