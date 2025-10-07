#ifndef ERROR_HANDLERS_H_INCLUDED
#define ERROR_HANDLERS_H_INCLUDED

enum error_type {
	NO_ERROR 			  = 0,
	NULL_ARG_ERROR 		  = 1 << 0,
	BIG_SIZE_ERROR 	      = 1 << 1,
	SMALL_SIZE_ERROR      = 1 << 2,
	NEG_SIZE_ERROR        = 1 << 3,
	NEG_CAP_ERROR         = 1 << 4,
	ZERO_CAP_ERROR        = 1 << 5,
	NULL_DATA_ERROR       = 1 << 6,
	MEM_ALLOC_ERROR       = 1 << 7,
	CANARY_DATA_ERROR     = 1 << 8,
	CANARY_STRUCT_ERROR   = 1 << 9,
	HASH_ERROR            = 1 << 10,
	PROCESSING_FILE_ERROR = 1 << 11,
	READ_FILE_ERROR 	  = 1 << 12,
	MEMORY_ALLOC_ERROR    = 1 << 13,
	SYNTAX_ERROR 		  = 1 << 14,
	NO_HLT_ERROR		  = 1 << 15,
	NULL_BUFF_ERROR		  = 1 << 16,
	LARGE_CMD_LEN  		  = 1 << 17,
	LARGE_ARGS_NUM 		  = 1 << 18
};

typedef long error_code;

#define RETURN_IF_ERROR(error_, ...) 					\
	do {											 	\
		if((error_) != 0) { 							\
			LOGGER_ERROR("Error code: %lu", (error_));  \
			__VA_ARGS__									\
			return (error_); 							\
		}												\
	} while(0)

#endif