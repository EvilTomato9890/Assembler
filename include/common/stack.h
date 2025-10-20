#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include "error_handlers.h"
#include "limits.h"
#include <stdlib.h>

//==============================================================================

#ifdef HASH_DEBUG
	#define VERIFY_DEBUG
	#define ON_HASH_DEBUG(...) __VA_ARGS__
#else
	#define ON_HASH_DEBUG(...)
#endif	

#ifdef CANARY_DEBUG
	#define VERIFY_DEBUG
	#define ON_CANARY_DEBUG(...) __VA_ARGS__
#else
	#define ON_CANARY_DEBUG(...)
#endif

#ifdef VERIFY_DEBUG //Почему нельзя ifdef внутри define
	#define ON_DEBUG(...) __VA_ARGS__
#else
	#define ON_DEBUG(...)
#endif

#define VER_INIT ver_data{__LINE__, 0, "int", "stack", __FILE__, __func__, "Init"}

//==============================================================================

typedef int st_type;

const int CANARY_NUM = INT_MAX;

struct ver_data {
	//-------------------Creation info
	int creation_line;
	unsigned long hash;
	const char* var_type;
	const char* var_name;
	const char* creation_file;
	const char* creation_func;
	const char* call_reason;
};

struct stack_t {
	ON_CANARY_DEBUG(
		int canary_begin;
	)

	st_type* data;
	st_type* original_ptr;
	ON_DEBUG(
		ver_data ver_info;
		bool is_constructed = true;
	)
	size_t size;
	size_t capacity;

	ON_CANARY_DEBUG(
		int canary_end;
	)
};

//==============================================================================

error_code stack_init(stack_t* stack_return, size_t capacity ON_DEBUG(, ver_data ver_info));

error_code stack_dest(stack_t* stack);

error_code stack_verify(const stack_t* const stack);

error_code stack_push(stack_t* stack, st_type elem);

st_type stack_pop(stack_t* stack, error_code* error_return); //Лучше возвращать ошибку или значение?

//==============================================================================
#endif