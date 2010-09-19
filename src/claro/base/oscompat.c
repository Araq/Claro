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


#include <claro/base.h>

#ifndef HAVE_MMAP
/*
 * static void mmap(void *hint, size_t len, uint32_t prot,
 *                  uint32_t flags, uint16_t fd, uint16_t off)
 *
 * Inputs       - mmap style arguments.
 * Output       - None
 * Side Effects - Memory is allocated to the block allocator.
 */
void *mmap(void *hint, size_t len, uint32_t prot,
		  uint32_t flags, uint16_t fd, uint16_t off)
{
	void *ptr = (void *)smalloc(len);

	if ( !ptr )
		clog( CL_CRITICAL, "mmap() call to smalloc() failed" );
	
	memset(ptr, 0, len);
	
	return ptr;
}

/*
 * static void munmap(void *addr, size_t len)
 *
 * Inputs       - munmap style arguments.
 * Output       - None
 * Side Effects - Memory is returned back to the OS.
 */
void munmap( void *addr, size_t len )
{
	if ( !addr )
		clog( CL_CRITICAL, "munmap() called with null address" );

	free(addr);
}
#endif

#ifdef _WIN32
#include <windows.h>
#endif

void mssleep( int ms )
{
#ifdef _WIN32
	Sleep( ms );
#else
	usleep( ms * 1000 );
#endif
}
