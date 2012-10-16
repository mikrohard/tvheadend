/*
 * dvb_ca_util.h
 *
 *  Created on: Jan 18, 2012
 *      Author: urosv
 *      TODO: This code is taken from dvblast.h source file of the DVBLAST ver 2.0 project.
 */

#ifndef DVB_CA_UTIL_H_
#define DVB_CA_UTIL_H_

#include <netdb.h>
#include <sys/socket.h>
#include <stdbool.h>

#define HAVE_CLOCK_NANOSLEEP
#define HAVE_ICONV


#define MAX_PIDS 8192

#define DEFAULT_VERBOSITY 3
#define VERB_DBG  4
#define VERB_INFO 3
#define VERB_WARN 2
#define VERB_ERR 1

typedef int64_t mtime_t;

typedef struct ts_pid_info {
    mtime_t  i_first_packet_ts;         /* Time of the first seen packet */
    mtime_t  i_last_packet_ts;          /* Time of the last seen packet */
    unsigned long i_packets;            /* How much packets have been seen */
    unsigned long i_cc_errors;          /* Countinuity counter errors */
    unsigned long i_transport_errors;   /* Transport errors */
    unsigned long i_bytes_per_sec;      /* How much bytes were process last second */
    uint8_t  i_scrambling;              /* Scrambling bits from the last ts packet */
    /* 0 = Not scrambled
       1 = Reserved for future use
       2 = Scrambled with even key
       3 = Scrambled with odd key */
} ts_pid_info_t;

extern int i_adapter;
extern mtime_t i_wallclock;
extern enum print_type_t i_print_type;


/* */
__attribute__ ((format(printf, 2, 3))) void msg_Info( void *_unused, const char *psz_format, ... );
__attribute__ ((format(printf, 2, 3))) void msg_Err( void *_unused, const char *psz_format, ... );
__attribute__ ((format(printf, 2, 3))) void msg_Warn( void *_unused, const char *psz_format, ... );
__attribute__ ((format(printf, 2, 3))) void msg_Dbg( void *_unused, const char *psz_format, ... );
__attribute__ ((format(printf, 2, 3))) void msg_Raw( void *_unused, const char *psz_format, ... );

mtime_t mdate( void );

#endif /* DVB_CA_UTIL_H_ */
