#include "BasicException.h"
#include <stdio.h>
#include <stdlib.h>

void g_SetNewHandler()
{
	// Set new handler for memory allocation failures
	// For SDL backend, this is a stub
}

void g_BasicException(EXCEPTION_CODE code, char* sz_error, char* file, unsigned long line)
{
	// Basic exception handler
	if (sz_error == NULL)
	{
		switch (code)
		{
			case MEM_ALLOC:
				fprintf(stderr, "Memory allocation failed at %s:%lu\n", file, line);
				break;
			case FILE_OPEN:
				fprintf(stderr, "File open failed at %s:%lu\n", file, line);
				break;
			case FILE_LOAD:
				fprintf(stderr, "File load failed at %s:%lu\n", file, line);
				break;
			case FAILED_JOB:
				fprintf(stderr, "Job failed at %s:%lu\n", file, line);
				break;
			case NULL_REF:
				fprintf(stderr, "Null reference at %s:%lu\n", file, line);
				break;
			default:
				fprintf(stderr, "Unknown error at %s:%lu\n", file, line);
				break;
		}
	}
	else
	{
		fprintf(stderr, "Error: %s at %s:%lu\n", sz_error, file, line);
	}

	// In debug mode, assert
	#ifdef _DEBUG
	assert(false);
	#else
	// In release mode, exit
	exit(1);
	#endif
}
