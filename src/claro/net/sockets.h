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


#ifndef _CLARO_NET_SOCKETS_H
#define _CLARO_NET_SOCKETS_H

enum cSocketErrors
{
	cSocketErrorHostnameLookup=1,
	cSocketErrorSocketCreate=2,
	cSocketErrorConnectionRefused=4,
	cSocketErrorSocketParameters=8,
	cSocketErrorClosed=16,
	cSocketErrorBindFailed=32,
	cSocketErrorListenFailed=64,
};

enum cSocketState
{
	cSocketDisconnected=0,
	cSocketError,
	cSocketLookup,
	cSocketConnecting,
	cSocketConnected,
	cSocketListening,
	cSocketClientWaiting,
};

typedef struct
{
	object_t object;
	
	list_t sockets;
	
	int maxfd;
} claro_net_t;

extern claro_net_t *claro_net;

typedef struct socket_ socket_t;

struct socket_
{
	object_t object;
	
	int status, error;
	
#ifdef _WIN32
	SOCKET s;
	int nret;
	LPHOSTENT hostEntry;
	SOCKADDR_IN serverInfo;
#else
	int s;
	int nret;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	
	struct sockaddr client_addr;
#endif
};

CLFEXP void claro_net_init( );

CLFEXP object_t *socket_create( object_t *parent, int flags );
CLFEXP void socket_connect( object_t *sock, char *hostname, int port );
CLFEXP void socket_listen( object_t *sock, char *hostname, int port );
CLFEXP object_t *socket_accept( object_t *server );
CLFEXP void socket_set_state( object_t *sock, int state );
CLFEXP void socket_close( object_t *sock );
CLFEXP int socket_error( object_t *sock );

#endif
