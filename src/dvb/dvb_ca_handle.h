/*
 * dvb_ca_handle.h
 *
 *  Created on: Dec 12, 2011
 *      Author: urosv
 */

#ifndef DVB_CA_HNADLE_H_
#define DVB_CA_HNADLE_H_

#include <stdint.h>
#include <stdbool.h>
#include <queue.h>
#include <bits/pthreadtypes.h>

#include "tvheadend.h"
#include "dvb.h"

#define CA_POLL_PERIOD 100000 /* 100 ms */

/*
 * Some minor functions to support the en50221.c functionality
 * */
void demux_ResendCAPMTs( void );
char *demux_Iconv(void *_unused, const char *psz_encoding,
                  char *p_string, size_t i_length);




#define MAX_POLL_TIMEOUT 100000 /* 100 ms */
void dvb_adapter_ca_init(void *aux);
void * dvb_ca_control(void *aux);


/*
 * PMT CA descrambling commands support
 * */
#define MAX_PMTCMD_BUF_SIZE 4096
typedef enum EdelayedPMTaction {
	eacPMTundefined,
	eacPMTadd,
	eacPMTdelete,
	eacPMTupdate
} e_delayed_PMT_action_t;
/**/
typedef struct pmtcmd_item {
	SIMPLEQ_ENTRY(pmtcmd_item)	link;
	e_delayed_PMT_action_t action;
	uint8_t p_pmt_buffer[MAX_PMTCMD_BUF_SIZE];
} pmtcmd_item_t;
/*
 * PMT CA commands are put into a que for single thread CA device handling*/
SIMPLEQ_HEAD(pmtcmd_pending_que_type, pmtcmd_item) pmtcmd_pending_que;
pthread_mutex_t pmtcmd_pending_que_mutex;
/*
 * Functions for PMT CA scrambling support*/
bool add_delete_update_PMT_delayed(uint8_t *p_pmt, e_delayed_PMT_action_t pmtaction);
int process_pending_PMTs(void);
bool start_transport_descrambling(struct service *t);
bool stop_transport_descrambling(struct service *t);
void ResetCAM(th_dvb_adapter_t *tda);
void clear_pending_PMTs(void);



#endif /* DVB_CA_HNADLE_H_ */
