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


#include <claro/net.h>

event_handler( client_socket_disconnected )
{
	printf( "Client closed connection.\n" );
}

event_handler( client_socket_error )
{
	printf( "Client socket error.\n" );
}

event_handler( client_waiting )
{
	object_t *client;
	
	while ( (client = socket_accept(object)) != NULL ) {
		/* we need to add handlers to the new client socket */
		object_addhandler( client, "disconnected", client_socket_disconnected );
		object_addhandler( client, "error", client_socket_error );
		
		printf( "Client connected!\n" );
	}
}

int main( int argc, char *argv[] )
{
	object_t *sock;
	
	claro_base_init( );
	claro_net_init( );
	
	sock = socket_create( NULL, 0 );
	object_addhandler( sock, "client-waiting", client_waiting );
	
	socket_listen( sock, "0.0.0.0", 11000 );
	
	claro_loop( );
	
	return 0;
}
