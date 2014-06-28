/*
 * shm_twins_send.h
 *
 *  Created on: 14 Dec 2013
 *      Author: heracles
 */

#ifndef __NTT_SHM_TWINS_SEND_H_
#define __NTT_SHM_TWINS_SEND_H_

#include "../../def.h"
#include "shm_twins_data.h"
#ifdef __cplusplus
extern "C" {
#endif
	int shm_twins_send(const char*);
	int shm_twins_send_init(char * key);
	int shm_twins_send_unlink();
	int shm_twins_send_sig();
	int shm_twins_send_write(char * key);
//	int shm_twins_send_write(int count);

#ifdef __cplusplus
}
#endif




#endif /* SHM_TWINS_SEND_H_ */
