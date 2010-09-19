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

extern context_t *ctx_main;

/*
 * Program loader.
 */
int main(int argc, char **argv)
{
	void (*i_func)(void);
	int iter;

	claro_base_init();

	log_fd_set_level( CL_DEBUG, stderr );

	clog(CL_DEBUG, "bootstrapping %s ..", ctx_main->context_name);

	/* ok, lets step through the init funcs */
	for (iter = 0; ctx_main->init_steps[iter] != NULL; iter++)
	{
		i_func = ctx_main->init_steps[iter];

		if (i_func != NULL)
			i_func();
	}

	claro_loop();

	return 0;
}
