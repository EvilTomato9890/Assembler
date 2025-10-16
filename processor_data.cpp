#include "stack.h"
#include "logger.h"
#include "error_handlers.h"
#include "input.h"
#include "processor_data.h"
#include "asserts.h"

//==============================================================================

error_code processor_data_dest(processor_data_t* data) {
	LOGGER_DEBUG("processor_data_dest started");

	HARD_ASSERT(data != nullptr, "data is nullptr");

	error_code error = 0;
	error |= stack_dest(data->stack);
	free(data->num_commands);
	return error;
}

error_code processor_data_init(processor_data_t* data, const char* file_name, stack_t* stack) {
	LOGGER_DEBUG("processor_data_init started");

	HARD_ASSERT(data != nullptr, "data is nullptr");

	error_code error = 0;

	FILE* input_file = fopen(file_name, "r");
	if(input_file == nullptr) return READ_FILE_ERROR;

	char* file_buff = nullptr;
	error |= read_file_into_buffer(input_file, &file_buff);
	RETURN_IF_ERROR(error, fclose(input_file););
	LOGGER_DEBUG("BUFF: %s", file_buff);

	error |= stack_init(stack, 5, VER_INIT);
	RETURN_IF_ERROR(error, fclose(input_file); free(file_buff););

	for(int i = 0; i < REGISTERS_NUM; i++) {
		data->registers[i] = 0;
	}
	data->num_commands = file_buff;
	data->stack        = stack;
	fclose(input_file);
	return NO_ERROR;
}


