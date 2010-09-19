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


#include <claro/contextual.h>

static void print_hello_world(void)
{
	clog(CL_DEBUG, "Hello world!");
}

void *c_init_steps[] = { &print_hello_world, NULL };

context_t contextual_demo = {
	.context_name = "Contextual Demo",
	.init_steps = c_init_steps,
};

DECLARE_ROOT_CONTEXT(contextual_demo);
