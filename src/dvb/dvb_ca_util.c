/*****************************************************************************
 * dvb_ca_util.c
 *****************************************************************************
 * Copyright (C) 2004 VideoLAN
 *
 * Authors: Christophe Massiot <massiot@via.ecp.fr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <syslog.h>

#include "dvb_ca_util.h"
#include "../libbitstream/mpeg/psi.h"
#include "../tvheadend.h"	/*Just needed for tvhlog(.) function*/


/*****************************************************************************
 * Local declarations
 *****************************************************************************/
#define MAX_MSG 1024

int i_adapter = 0;
mtime_t i_wallclock = 0;

print_type_t i_print_type = -1;
int i_syslog = 0;
int i_verbose = DEFAULT_VERBOSITY;

/*****************************************************************************
 * msg_Info
 *****************************************************************************/
void msg_Info( void *_unused, const char *psz_format, ... )
{
    if ( i_verbose >= VERB_INFO )
    {
        char buf[MAX_MSG];
        va_list args;
        va_start( args, psz_format );
        vsnprintf(buf, sizeof(buf), psz_format, args);
        va_end(args);

        tvhlog(LOG_INFO, "dvb", "%s", buf);
    }
}

/*****************************************************************************
 * msg_Err
 *****************************************************************************/
void msg_Err( void *_unused, const char *psz_format, ... )
{
    if ( i_verbose >= VERB_ERR )
    {
        char buf[MAX_MSG];
        va_list args;
        va_start( args, psz_format );
        vsnprintf(buf, sizeof(buf), psz_format, args);
        va_end(args);

        tvhlog(LOG_ERR, "dvb", "%s", buf);
    }
}

/*****************************************************************************
 * msg_Warn
 *****************************************************************************/
void msg_Warn( void *_unused, const char *psz_format, ... )
{
    if ( i_verbose >= VERB_WARN )
    {
    	char buf[MAX_MSG];
		va_list args;
		va_start( args, psz_format );
		vsnprintf(buf, sizeof(buf), psz_format, args);
		va_end(args);

		tvhlog(LOG_WARNING, "dvb", "%s", buf);
    }
}

/*****************************************************************************
 * msg_Dbg
 *****************************************************************************/
void msg_Dbg( void *_unused, const char *psz_format, ... )
{
    if ( i_verbose >= VERB_DBG )
    {
    	char buf[MAX_MSG];
    	va_list args;
    	va_start( args, psz_format );
    	vsnprintf(buf, sizeof(buf), psz_format, args);
    	va_end(args);

    	tvhlog(LOG_DEBUG, "dvb", "%s", buf);
    }
}

/*****************************************************************************
 * msg_Raw
 *****************************************************************************/
void msg_Raw( void *_unused, const char *psz_format, ... )
{
	char buf[MAX_MSG];
   	va_list args;
   	va_start( args, psz_format );
   	vsnprintf(buf, sizeof(buf), psz_format, args);
   	va_end(args);

   	tvhlog(LOG_NOTICE, "dvb", "%s", buf);
}

/*****************************************************************************
 * mdate
 *****************************************************************************/
mtime_t mdate( void )
{
#if defined (HAVE_CLOCK_NANOSLEEP)
    struct timespec ts;

    /* Try to use POSIX monotonic clock if available */
    if( clock_gettime( CLOCK_MONOTONIC, &ts ) == EINVAL )
        /* Run-time fallback to real-time clock (always available) */
        (void)clock_gettime( CLOCK_REALTIME, &ts );

    return ((mtime_t)ts.tv_sec * (mtime_t)1000000)
            + (mtime_t)(ts.tv_nsec / 1000);
#else
    struct timeval tv_date;

    /* gettimeofday() could return an error, and should be tested. However, the
     * only possible error, according to 'man', is EFAULT, which can not happen
     * here, since tv is a local variable. */
    gettimeofday( &tv_date, NULL );
    return( (mtime_t) tv_date.tv_sec * 1000000 + (mtime_t) tv_date.tv_usec );
#endif
}


