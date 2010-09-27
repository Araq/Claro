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


#ifndef _CLARO_BASE_H
#define _CLARO_BASE_H

#ifdef _NIX
typedef unsigned short bool;
#endif

#ifndef false
#define false 0
#endif

#ifndef true
#define true !(false)
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE !(FALSE)
#endif


#ifdef __cplusplus
extern "C" {
#endif

#include "stdinc.h"
#include "list.h"
#include "block.h"
#include "object.h"
#include "claro.h"
#include "log.h"
#include "memory.h"
#include "store.h"
#include "object_map.h"
#include "string.h"
//added by Cody, 2/8/06
#include "hashtable.h"

#ifdef __cplusplus
}
#endif

#endif
