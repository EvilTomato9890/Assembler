#include <string.h>

#include "logger.h"
#include "asserts.h"
#include "input.h"
#include "assembler.h"
#include "processor_data.h"

	
//static command_num choose_command(command_data* command);



static command_type command_arr[] = {   					
										{"PUSH", cmdPUSH},  		
									    {"ADD",  cmdADD},   
									    {"SUB",  cmdSUB},
										{"DEL",  cmdDEL},
										{"MULT", cmdMULT},
										{"POW",  cmdPOW},
										{"SQRT", cmdSQRT},
										{"OUT",  cmdOUT},
										{"PUSHR",cmdPUSHR},
										{"POPR", cmdPOPR},
										{"HLT",  cmdHLT},
										{"JE",   cmdJE},
										{"JB",   cmdJB},
										{"JA",   cmdJA},
										{"JUMP", cmdJUMP},
										{"IN",   cmdIN}
									};

//==============================================================================

static command_num choose_command(command_data* command) {
	HARD_ASSERT(command != nullptr, "Command is nullptr");
	HARD_ASSERT(command->str != nullptr, "Command_str is nullptr");

	for(int i = 0; i < COMMAND_TYPES_NUM; i++) {
		if(strncmp(command->str, command_arr[i].cmd_name, command->command_len) == 0) {
			return command_arr[i].cmd;
		}
	}

	LOGGER_ERROR("Unknown cmd: %s",  command->str);
	return cmdHLT;
}

//==============================================================================

error_code assemble(FILE* input_file, FILE* command_file) {
	LOGGER_DEBUG("assemble started");

	HARD_ASSERT(input_file != nullptr, "Input file is nullptr");
	HARD_ASSERT(command_file != nullptr, "command_file is nullptr");

	text_data text = {};
	error_code error = 0;

	char* file_buff = nullptr;
	error |= read_file_into_buffer(input_file, &file_buff); 
	char* original_ptr = file_buff;
	RETURN_IF_ERROR(error, free(original_ptr); fclose(input_file););

    error |= text_init(file_buff, &text); 
    command_num command = cmdHLT;


    for(size_t i = 0; i < text.str_num; i++) {
    	command = choose_command(text.commands[i]);
    	LOGGER_DEBUG("Command: %d", command);
    	fprintf(command_file, "%d", command);
    	if(command == cmdPUSHR || command == cmdPOPR) {
    		fprintf(command_file, " %d\n", *(strchr(text.commands[i]->str, 'R') + 3) - 'A');
    	} else {
    		char* args = strchr(text.commands[i]->str, ' ');
    		if(args != nullptr) fprintf(command_file, "%s\n", args);
    		else				fprintf(command_file, "\n");
    	}
    }

    text_dest(&text);
    free(original_ptr);
    if(error != NO_ERROR) return error;
    return error;
}
