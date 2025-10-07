#include "logger.h"
#include "asserts.h"
#include "processor.h"
#include "input.h"
#include "processor_data.h"

#include <string.h>
#include <math.h>
#include <stdlib.h>

//==============================================================================

static error_code add_func (stack_t* stack);
static error_code push_func(stack_t* stack, char* args);
static error_code sub_func (stack_t* stack);
static error_code del_func (stack_t* stack);
static error_code mult_func(stack_t* stack);
static error_code pow_func (stack_t* stack);
static error_code sqrt_func(stack_t* stack);
static error_code out_func (stack_t* stack, FILE* output_file);

//==============================================================================


static error_code push_func(stack_t* stack, char* args) {
	error_code error = 0;
	st_type first = 0;
	sscanf(args, "%d", &first);
	error |= stack_push(stack, first);
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
	fprintf(output_file, "%d", first);
	return error;
}


static error_code execute_command(int command, char* args, stack_t* stack, bool* is_HLT, FILE* output_file) {
	HARD_ASSERT(stack != nullptr, "Stack is nullptr");

	error_code error = 0;

	switch(command) {
		case cmdPUSH:  error |= push_func(stack, args);        break;
		case cmdADD:   error |= add_func (stack); 		       break;
		case cmdSUB:   error |= sub_func (stack);              break;
		case cmdDEL:   error |= del_func (stack);          	   break;
		case cmdMULT:  error |= mult_func(stack);              break;
		case cmdPOW:   error |= pow_func (stack);              break;
		case cmdSQRT:  error |= sqrt_func(stack);       	   break;
		case cmdOUT:   error |= out_func (stack, output_file); break;
		case cmdHLT:   *is_HLT = true;						   break;
		default:
			error |= SYNTAX_ERROR;
			break;

	}
	return error;
}

error_code execution(FILE* output_file, processor_data_t* processor_data) {
	LOGGER_DEBUG("execution started");

	HARD_ASSERT(processor_data != nullptr, "processor_data is nullptr");
	HARD_ASSERT(output_file != nullptr, "output_file is nullptr");

	text_data text = {};
	error_code error = 0;
	stack_dumb(processor_data->stack);
	RETURN_IF_ERROR(error, LOGGER_ERROR("AAAAA"););
    error |= text_init(processor_data->num_commands, &text); 
    bool is_HLT = false;

	LOGGER_WARNING("STACK_SIZE: %lu, STACK_CAPACITY: %lu, STACK_DATA: %p\n STACK_FIRST_ELEM:", processor_data->stack->size, 
					processor_data->stack->capacity, processor_data->stack->data);
    for(size_t i = 0; i < text.str_num; i++) {
    	char* args = 0;
    	//long command = strtol(text.commands[i]->str, &args, 10);
    	int command = (int)strtol(text.commands[i]->str, &args, 10);
    	error |= execute_command(command, args, 
    							 processor_data->stack, &is_HLT, output_file);
    	RETURN_IF_ERROR(error, text_dest(&text););
    	if(is_HLT) break;
    }
    if(!is_HLT) error |= NO_HLT_ERROR;

    text_dest(&text);
    if(error != NO_ERROR) return error;
    return error;
}
