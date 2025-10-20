#include "common/stack.h"
#include "common/logger.h"
#include "common/error_handlers.h"
#include "common/input.h"
#include "cpu/processor_data.h"
#include "common/asserts.h"

#include <ctype.h>
static size_t count_commands(char* buff);

//==============================================================================

error_code processor_data_dest(processor_data_t* data) {
	LOGGER_DEBUG("processor_data_dest started");

	HARD_ASSERT(data != nullptr, "data is nullptr");

	error_code error = 0;
	error |= stack_dest(data->stack);
	free(data->commands_buff);
	return error;
}

static size_t count_commands(char* buff) {
	LOGGER_DEBUG("count_commands started");
    HARD_ASSERT(buff != nullptr, "Buff is nullptr");
    
    size_t cmd_num = 0;
    size_t index = 0;

    if(buff[index] == '\0') {
    	LOGGER_WARNING("Empty buff");
    	return 0;
    }

    while(buff[index] != '\0') {
        if(isblank(buff[index]) && !isblank(buff[index - 1])) {
            cmd_num++;
        }
        index++;
    }
    if (cmd_num <= 1) LOGGER_WARNING("Too less commands");
    return cmd_num;
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

	data->commands_num = count_commands(file_buff);
	LOGGER_DEBUG("Commands_num: %ld", data->commands_num);

	for(int i = 0; i < REGISTERS_NUM; i++) {
		data->registers[i] = 0;
	}
	data->commands_buff = file_buff;
	data->stack         = stack;
	fclose(input_file);
	return NO_ERROR;
}


