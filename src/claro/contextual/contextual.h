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


#ifndef _CLARO_CONTEXTUAL_CONTEXTUAL_H
#define _CLARO_CONTEXTUAL_CONTEXTUAL_H

typedef struct context_initializer_ context_t;
typedef struct context_type_ context_type_t;

struct context_type_ {
	int (*initialize)(context_t *);
};

struct context_initializer_ {
	char *context_name;
	int argc;
	char **argv;
	void **init_steps;
	void **deinit_steps;
	context_type_t *program_initializer;
	void *data;
};

#define DECLARE_ROOT_CONTEXT(x)				\
	context_t *ctx_main = &(x);

#endif
