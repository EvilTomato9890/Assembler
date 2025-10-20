#include "logger.h"
#include "asserts.h"
#include "common.h"
#include "input.h"
#include "processor_data.h"
#include "processor.h"

#include <string.h>
#include <math.h>
#include <stdlib.h>

//==============================================================================

static error_code add_func (stack_t* stack);
static error_code push_func(stack_t* stack, st_type* args);
static error_code sub_func (stack_t* stack);
static error_code del_func (stack_t* stack);
static error_code mult_func(stack_t* stack);
static error_code pow_func (stack_t* stack);
static error_code sqrt_func(stack_t* stack);
static error_code out_func (stack_t* stack, FILE* output_file);

static error_code input_commands(char* buff, size_t commands_num, st_type** return_commands);

//==============================================================================

static error_code push_func(stack_t* stack, st_type* args) { //todo: новый define 
	error_code error = 0;
	error |= stack_push(stack, *args);
	return error;
} 

static error_code add_func(stack_t* stack) {
	error_code error = 0;
	st_type first = stack_pop(stack, &error);
	st_type second = stack_pop(stack, &error);
	error |= stack_push(stack, second + first);
	return error;
}

static error_code sub_func(stack_t* stack) {
	error_code error = 0;
	st_type first = stack_pop(stack, &error);
	st_type second = stack_pop(stack, &error);
	error |= stack_push(stack, second - first);
	return error;
}

static error_code del_func(stack_t* stack) {
	error_code error = 0;
	st_type first = stack_pop(stack, &error);
	st_type second = stack_pop(stack, &error);
	error |= stack_push(stack, second / first);
	return error;
}

static error_code mult_func(stack_t* stack) {
	error_code error = 0;
	st_type first = stack_pop(stack, &error);
	st_type second = stack_pop(stack, &error);
	error |= stack_push(stack, second * first);
	return error;
}

static error_code pow_func(stack_t* stack) {
	error_code error = 0;
	st_type first = stack_pop(stack, &error);
	st_type second = stack_pop(stack, &error);
	error |= stack_push(stack, (st_type)pow(second, first));
	return error;
}

static error_code sqrt_func(stack_t* stack) {
	error_code error = 0;
	st_type first = stack_pop(stack, &error);
	error |= stack_push(stack, (st_type)sqrt(first));
	return error;
}

static error_code out_func(stack_t* stack, FILE* output_file) {
	error_code error = 0;
	st_type first = stack_pop(stack, &error);
	fprintf(output_file, "%d\n", first);
	return error;
}

static error_code pushr_func(processor_data_t* processor_data, st_type* args) {
	error_code error = 0;
	error |= stack_push(processor_data->stack, processor_data->registers[args[0]]);
	return error;
} 

static error_code popr_func(processor_data_t* processor_data, st_type* args) {
	error_code error = 0;
	st_type value = stack_pop(processor_data->stack, &error);
	processor_data->registers[args[0]] = value;
	return error;
} 

static error_code jump_func(st_type* args, size_t* index) {
	LOGGER_DEBUG("JMP STARTED");
	error_code error = 0;
	*index = args[0];
	return error;
}

static error_code je_func(stack_t* stack, st_type* args, size_t* index) {
	LOGGER_DEBUG("JE STARTED");
	error_code error = 0;
	st_type first = stack_pop(stack, &error);
	st_type second = stack_pop(stack, &error);
	if(first == second) {
		*index = args[0];
	}
	return error;
}

static error_code jb_func(stack_t* stack, st_type* args, size_t* index) {
	LOGGER_DEBUG("JB STARTED");
	error_code error = 0;
	st_type first = stack_pop(stack, &error);
	st_type second = stack_pop(stack, &error);
	if(first < second) {
		*index = args[0];
	}
	return error;
}

static error_code ja_func(stack_t* stack, st_type* args, size_t* index) {
	LOGGER_DEBUG("JA STARTED");
	error_code error = 0;
	st_type first  = stack_pop(stack, &error);
	st_type second = stack_pop(stack, &error);
	LOGGER_DEBUG("first: %d, second: %d", first, second);
	if(first > second) {
		*index = args[0] - 1;
		LOGGER_DEBUG("ind: %d", *index);
	}
	return error;
}


static error_code in_func(stack_t* stack) {
	error_code error = 0;
	st_type value = 0;
	scanf("%d", &value);
	error |= stack_push(stack, value);
	return error;
}

//------------------------------------------------------------------------------

static error_code execute_command(st_type* commands, processor_data_t* processor_data, 
								  bool* is_HLT, FILE* output_file, size_t* index) {
	HARD_ASSERT(processor_data != nullptr, "Stack is nullptr");
	HARD_ASSERT(commands != nullptr, "Stack is nullptr");
	HARD_ASSERT(is_HLT != nullptr, "is_HLT is nullptr");
	HARD_ASSERT(output_file != nullptr, "output_file is nullptr");
	HARD_ASSERT(index != nullptr, "index is nullptr");

	error_code error = 0;
	stack_t* stack = processor_data->stack;          //Обертки на функции памяти 
	// Записывать куда и что записало. Тогда санитайзер не нужен #Артем xuesoso




	command_num command = (command_num)*commands;
	switch(command) {
		case cmdPUSH:  error |= push_func (stack, commands+1);         *index += 1;  break;
		case cmdADD:   error |= add_func  (stack); 		          				     break;
		case cmdSUB:   error |= sub_func  (stack);                 				     break;
		case cmdDEL:   error |= del_func  (stack);          	      	 		     break;
		case cmdMULT:  error |= mult_func (stack);                 		 		     break;
		case cmdPOW:   error |= pow_func  (stack);                 		 		     break;
		case cmdSQRT:  error |= sqrt_func (stack);       	      				     break;
		case cmdOUT:   error |= out_func  (stack, output_file);   	 			     break;
		case cmdPUSHR: error |= pushr_func(processor_data, commands+1); *index += 1; break;
		case cmdPOPR:  error |= popr_func (processor_data, commands+1); *index += 1; break;
		case cmdIN:    error |= in_func   (stack);			     	  			     break;
		case cmdJUMP:  error |= jump_func (commands, index);           			     break;
		case cmdJE:    error |= je_func   (stack, commands+1, index);   *index += 1; break;
		case cmdJB:    error |= jb_func   (stack, commands+1, index);   *index += 1; break;
		case cmdJA:    error |= ja_func   (stack, commands+1, index);   *index += 1; break;
		case cmdLBL:  					 										     break; //todo: E,hfnm b ghjcnj buyjhbhjdfnm :_ d fcctv,kbhjdfyybb
		case cmdHLT:   *is_HLT = true;						      				     break;
		default:
			LOGGER_ERROR("SYNTAX_ERROR");
			error |= SYNTAX_ERROR;
			break;

	}
	return error;
}

//==============================================================================

static error_code input_commands(char* buff, size_t commands_num, st_type** return_commands) { //todo: st_type выглядит как ошибка (больше возможностей в будущем)
	LOGGER_DEBUG("input_commands started");

	HARD_ASSERT(buff != nullptr, "buff is nullptr");
	HARD_ASSERT(return_commands != nullptr, "return_commands is nullptr");

	error_code error = 0;
	char* next_ptr = nullptr;

	*return_commands = (st_type*)calloc(commands_num, sizeof(st_type));
	if (return_commands == nullptr) {
		LOGGER_ERROR("Mem allocation error");
		error |= MEM_ALLOC_ERROR;
	}
	RETURN_IF_ERROR(error);

	for(size_t i = 0; i < commands_num; i++) {
		(*return_commands)[i] = (st_type)strtol(buff, &next_ptr, 10);
		buff = next_ptr;
	}
	return error;
}

//==============================================================================

error_code execution(FILE* output_file, processor_data_t* processor_data) {
	LOGGER_DEBUG("execution started");

	HARD_ASSERT(processor_data != nullptr, "processor_data is nullptr");
	HARD_ASSERT(output_file != nullptr, "output_file is nullptr");

	error_code error = 0;

    st_type* commands = nullptr;
    error |= input_commands(processor_data->commands_buff, processor_data->commands_num, &commands);
    RETURN_IF_ERROR(error);

    bool is_HLT = false;
    for(size_t i = 0; i < processor_data->commands_num; i++) {
    	LOGGER_DEBUG("COMMAND[%d]: %d", i, *(commands + i));
    	error |= execute_command(commands + i, processor_data, &is_HLT, output_file, &i);
    	RETURN_IF_ERROR(error, free(commands););
    	if(is_HLT) break;
    }
    if(!is_HLT) error |= NO_HLT_ERROR;

    free(commands);
    return error;
}
//todo: ввод адреса памяти через значение в регистре (Артем xuesos)


//это помогает так как все переменные лежат рядом и переход x + 8 юзнается везде 