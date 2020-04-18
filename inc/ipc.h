/*
 * ipc.h
 *
 *  Created on: 10.03.2018
 *      Author: Admin
 */

#ifndef IPC_H_
#define IPC_H_

//*********** Includes **************

//*********** Defines **************
#define memory_data_size 512
#define did_size 	20


//*********** Functions **************
void* ipc_memory_register	(unsigned int no_bytes, unsigned char did);
void* ipc_memory_get		(unsigned char did);


#endif /* IPC_H_ */
