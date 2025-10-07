#include <string.h>

#include "logger.h"
#include "asserts.h"
#include "input.h"
#include "assembler.h"
#include "processor_data.h"

	
//static command_num choose_command(command_data* command);

//==============================================================================

static command_num choose_command(command_data* command) {
	HARD_ASSERT(command != nullptr, "Command is nullptr");
	HARD_ASSERT(command->str != nullptr, "Command_str is nullptr");

	if(strncmp(command->str, "PUSH", command->command_len) == 0)  return cmdPUSH;
	if(strncmp(command->str, "ADD",  command->command_len) == 0)  return cmdADD;
	if(strncmp(command->str, "SUB",  command->command_len) == 0)  return cmdSUB;
	if(strncmp(command->str, "DEL",  command->command_len) == 0)  return cmdDEL;
	if(strncmp(command->str, "MULT", command->command_len) == 0)  return cmdMULT;
	if(strncmp(command->str, "POW",  command->command_len) == 0)  return cmdPOW;
	if(strncmp(command->str, "SQRT", command->command_len) == 0)  return cmdSQRT;
	if(strncmp(command->str, "OUT",  command->command_len) == 0)  return cmdOUT;
	if(strncmp(command->str, "HLT",  command->command_len) == 0)  return cmdHLT;
	LOGGER_ERROR("Unknown cmd: %s",  command->str);
	return cmdHLT;
}

error_code assemble(FILE* input_file, FILE* command_file) {
	LOGGER_DEBUG("assemble started");

	HARD_ASSERT(input_file != nullptr, "Input file is nullptr");
	HARD_ASSERT(command_file != nullptr, "command_file is nullptr");

	text_data text = {};
	error_code error = 0;

	char* file_buff = 0;
	error |= read_file_into_buffer(input_file, &file_buff);
	char* original_ptr = file_buff;
	RETURN_IF_ERROR(error, free(original_ptr); fclose(input_file););

    error |= text_init(file_buff, &text); 
    int command = 0;

    for(size_t i = 0; i < text.str_num; i++) {
    	command = choose_command(text.commands[i]);
    	LOGGER_DEBUG("Command: %d", command);
    	fprintf(command_file, "%d", command);
    	char* args = strchr(text.commands[i]->str, ' ');
    	if(args != nullptr) fprintf(command_file, "%s\n", args);
    	else				fprintf(command_file, "\n");
    }

    text_dest(&text);
    free(original_ptr);
    if(error != NO_ERROR) return error;
    return error;
}
