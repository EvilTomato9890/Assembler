#include <string.h>

#include "logger.h"
#include "asserts.h"
#include "input.h"
#include "assembler.h"
#include "processor_data.h"
#include "common.h"
//static command_num choose_command(command_data* command);

//==============================================================================
//todo: Добавить парсинг табов в asm
command_info command_arr[] = {   					 //добавить .n_args = 
								{"PUSH", cmdPUSH,  1},  		
							    {"ADD",  cmdADD,   0},   
							    {"SUB",  cmdSUB,   0},
								{"DEL",  cmdDEL,   0},
								{"MULT", cmdMULT,  0},
								{"POW",  cmdPOW,   0},
								{"SQRT", cmdSQRT,  0},
								{"OUT",  cmdOUT,   0},
								{"PUSHR",cmdPUSHR, 1},
								{"POPR", cmdPOPR,  1},
								{"HLT",  cmdHLT,   0},
								{"JE",   cmdJE,    1},
								{"JB",   cmdJB,    1},
								{"JA",   cmdJA,    1},
								{"JUMP", cmdJUMP,  1},
								{"IN",   cmdIN,    0},
								{":",    cmdLBL,   1}
							};

//==============================================================================

error_code assembler_info_init(assembler_info_t* asm_info, FILE* command_file, FILE* asm_file) {
	HARD_ASSERT(asm_info     != nullptr, "Asm_info is nullptr");
	HARD_ASSERT(command_file != nullptr, "command_file is nullptr");
	HARD_ASSERT(asm_file     != nullptr, "asm_file is nullptr");

	LOGGER_DEBUG("init_assembler_info started");

	error_code error = 0;

	char* file_buff = nullptr;
	error |= read_file_into_buffer(command_file, &file_buff); 
	RETURN_IF_ERROR(error, free(file_buff););

    error |= text_init(file_buff, &(asm_info->text)); 
    RETURN_IF_ERROR(error, free(file_buff););

    asm_info->buff 		   = file_buff;
    asm_info->command_file = command_file;
    asm_info->asm_file     = asm_file;
    for(int i = 0; i < LABELS_CNT; i++) {
    	asm_info->labels[i] = -1;
    }

    return error;
}

error_code assembler_info_dest(assembler_info_t* asm_info) {
	HARD_ASSERT(asm_info != nullptr, "Asm_info = nullptr");

	LOGGER_DEBUG("assembler_info_dest started");

	error_code error = 0;

	error |= text_dest(&(asm_info->text));
    free(asm_info->buff);
    return error;
}
//==============================================================================

static command_num choose_command(const command_data* command) {
	HARD_ASSERT(command != nullptr, "Command is nullptr");
	HARD_ASSERT(command->str != nullptr, "Command_str is nullptr");

	for(int i = 0; i < COMMAND_TYPES_NUM; i++) {
		if(strncmp(command->str, command_arr[i].cmd_name, command->command_len) == 0) {
			return command_arr[i].cmd;
		}
	}

	LOGGER_ERROR("Unknown cmd: %s",  command->str); //todo: Добавить cmdUnk bkb nbgf njuj
	return cmdHLT;
}

union args_t {
	int     i_num;
	double  d_num;
	char*  	str;
};

enum arg_type_t {
	STRING,
	INTEG,
	DOUBLE,
	UNDEFINED
};

//todo: hueten'!
static args_t get_args(command_data* command, arg_type_t* arg_type) {
	HARD_ASSERT(command != nullptr, "command is nullptr");

	args_t args = {};
	if     (sscanf(command->str + command->command_len, "%d",  &(args.i_num)) ==  1)  {*arg_type = INTEG;  return args;}
	else if(sscanf(command->str + command->command_len, "%lf", &(args.d_num)) ==  1)  {*arg_type = DOUBLE; return args;}
	//else if(sscanf(command->str + command->command_len, "%29s",  &(args.str))   != -1)  {*arg_type = STRING; return args;}
	
	*arg_type = UNDEFINED;
	return args;
}

#define RETURN_IF_WRONG_TYPE(error, type, needed_type) \
	if(type != needed_type) {						   \
		LOGGER_ERROR("SYNTAX_ERROR");        		   \
		error |= SYNTAX_ERROR;						   \
		return error;								   \
	}

//==============================================================================

static error_code second_assembler_run(assembler_info_t asm_info) { //todo: Отказаться от структуры и оставить char

	error_code error = 0;

	command_num command = cmdHLT;

	for(size_t i = 0; i < asm_info.text.str_num; i++) {
    	arg_type_t arg_type = STRING;

    	command     = choose_command(asm_info.text.commands[i]);
    	args_t args = get_args(asm_info.text.commands[i], &arg_type); 

    	LOGGER_DEBUG("Command: %d", command);
    	LOGGER_DEBUG("Args: %d, arg_type: %d", args.i_num, arg_type);
    	fprintf(asm_info.asm_file, "%d ", command);
    	if(command == cmdPUSHR || command == cmdPOPR) {
    		//RETURN_IF_WRONG_TYPE(error, arg_type, STRING);

    		fprintf(asm_info.asm_file, "%d ", *(strchr(asm_info.text.commands[i]->str + asm_info.text.commands[i]->command_len, 'R') + 1) - 'A'); //todo: Проверку
    	} else if(command == cmdJA || command == cmdJB || command == cmdJE || command == cmdJUMP){
    		//RETURN_IF_WRONG_TYPE(error, arg_type, STRING);

    		size_t cmd_address = *(strchr(asm_info.text.commands[i]->str + asm_info.text.commands[i]->command_len, ':') + 1) - '0'; //todo: На числа
    		fprintf(asm_info.asm_file, "%d ", asm_info.labels[cmd_address]);
    		LOGGER_DEBUG("%d ", asm_info.labels[cmd_address]);
    	} else if(command == cmdPUSH){
    		RETURN_IF_WRONG_TYPE(error, arg_type, INTEG);

    		fprintf(asm_info.asm_file, "%d ", args.i_num);
    		LOGGER_DEBUG("%d ", args.i_num);
    	} 
    }
    return error;
}

static error_code first_assembler_run(assembler_info_t* asm_info) {
	
	error_code error = 0;

	command_num command       = cmdHLT;
    size_t cmd_address        = 0;

	for(size_t i = 0; i < asm_info->text.str_num; i++) {
    	arg_type_t arg_type = STRING;

    	command     = choose_command(asm_info->text.commands[i]);
    	args_t args = get_args(asm_info->text.commands[i], &arg_type); 

    	if(command == cmdPUSHR || command == cmdPOPR ||
    	   command == cmdJA    || command == cmdJB   || command == cmdJE || command == cmdJUMP ||
    	   command == cmdPUSH) {								  
    		cmd_address++;
    	}else if(command == cmdLBL && asm_info->labels[args.i_num] != 0) {
    		asm_info->labels[args.i_num] = cmd_address;
    	}
    	cmd_address++;
    }
    return error;
}

error_code assemble(FILE* command_file, FILE* asm_file) {
	LOGGER_DEBUG("assemble started");

	HARD_ASSERT(command_file != nullptr, "command_file is nullptr");
	HARD_ASSERT(asm_file != nullptr, "asm_file is nullptr");

	error_code error = 0;

	assembler_info_t asm_info = {};

    error |= assembler_info_init(&asm_info, command_file, asm_file);
    error |= first_assembler_run(&asm_info);

    for(int i = 0; i < LABELS_CNT; i++) {
    	LOGGER_DEBUG("[%d]%d", i, asm_info.labels[i]);
    }
    error |= second_assembler_run(asm_info);

    error |= assembler_info_dest(&asm_info);
    
    if(error != NO_ERROR) return error;
    return error;
}

#undef RETURN_IF_WRONG_TYPE
/*
HLT
PUSH 10
PUSH 10	
IN
PUSHR RAX
IN
PUSHR RBX
POPR RAX
POPR RBX
JE 0
OUT
POPR RBX
OUT
HLT
*/