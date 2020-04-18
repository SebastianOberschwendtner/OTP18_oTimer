/*
 * ipc.c
 *
 *  Created on: 17.04.2020
 *      Author: Sebastian
 */

// ********* Includes *********
#include "ipc.h"


// ********* Variables *********

unsigned char 	ipc_data_memory[memory_data_size];
unsigned int	memory_data_idx = 0;

void* memory_data_did[did_size];

//************************************
//*********** Functions **************

// register Data Memory and return pointer
void* ipc_memory_register(unsigned int no_bytes, unsigned char did)
{
	unsigned int temp 	= memory_data_idx;
	memory_data_idx += no_bytes;

	if(memory_data_idx < memory_data_size)					// Check for enough Memory
	{
		memory_data_did[did] = (void*) &ipc_data_memory[temp];
		return (void*) &ipc_data_memory[temp];
	}
	else
	{
	//	error = err_no_memory_left;
		return 0;
	}
};

// get pointer of registered Memory
void* ipc_memory_get(unsigned char did)
{
	return memory_data_did[did];
};
