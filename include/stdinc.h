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


#ifndef _CLARO_BASE_STDINC_H
#define _CLARO_BASE_STDINC_H

#ifdef _WIN32
#	ifdef PLUGIN
#		define CLFEXP __declspec (dllimport)
#	else
#		define CLFEXP __declspec (dllexport)
#	endif
#	define CLVEXP extern CLFEXP
#else
#	define CLFEXP
#	define CLVEXP extern
#endif

typedef unsigned int uint32;

#ifdef _WIN32
typedef uint32 uint32_t;
typedef unsigned short uint16_t;
#endif

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*#include <stdint.h> */
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <setjmp.h>
#include <sys/stat.h>

/* for windows */
#ifndef SIGQUIT
#define SIGQUIT SIGABRT
#endif

#ifndef SIGUSR1
#define SIGUSR1 SIGABRT
#endif

#endif
