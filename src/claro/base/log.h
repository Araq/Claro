/* Claro Graphics - an abstraction layer for native UI libraries
 * 
 * $Id$
 * 
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 * 
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
 * License for the specific language governing rights and limitations
 * under the License.
 * 
 * See the LICENSE file for more details.
 */


#ifndef _CLARO_BASE_LOG_H
#define _CLARO_BASE_LOG_H

enum {
	/* Nothing should be actually logged as CL_ANY */
	CL_ANY = 0,
	
	/* Stuff that you generally don't want to see */
	CL_DEBUG,
	
	/* Information, not an error */
	CL_INFO,
	
	/* A warning that something might be wrong, not an error */
	CL_WARNING,
	
	/* Something is wrong, but we can still continue */
	CL_ERROR,
	
	/* Something is so bad we have to stop immediately */
	CL_CRITICAL,
};

/* log location item */
typedef struct log_type_ log_type_t;
struct log_type_
{
	int level;
	
	FILE *fd;
};

/* If no application defined, assume unknown or Claro */
#ifndef CLARO_APPLICATION
/* Define application as claro if this it's Claro's source being compiled. */
#ifdef CLARO_SOURCE
#define CLARO_APPLICATION "claro"
#else
#define CLARO_APPLICATION "unknown_application"
#endif
#endif

#ifdef PRODUCTION
#define clog(a,b,...)
#else
#define clog(l,f,args...) claro_log( l, CLARO_APPLICATION, __FILE__, __LINE__, __FUNCTION__, f, ## args )
#endif

CLFEXP void log_init( );
CLFEXP void log_add_fd( int level, FILE *fd );
CLFEXP void claro_log( int level, char *application, char *file, int line, const char *function, const char *fmt, ... );
CLFEXP void log_fd_set_level( int level, FILE *fd );

#endif
