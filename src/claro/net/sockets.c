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

char *cSocketEvents[] = {
	"disconnected",
	"error",
	"lookup",
	"connecting",
	"connected",
	"listening",
	"client-waiting",
};

#define cSocketSelectTime 10

claro_net_t *claro_net = NULL;
event_handler( claro_net_loop );

/* Initialise claro.net */
void claro_net_init( )
{
#ifdef _WIN32
	WORD sockVersion;
	WSADATA wsaData;
	
	sockVersion = MAKEWORD( 1, 1 );
	WSAStartup( sockVersion, &wsaData );
#endif
	
	claro_net = (claro_net_t *)object_create( claro, sizeof(claro_net_t), "claro.net" );
	list_create( &claro_net->sockets );
	claro_net->maxfd = 0;
	
	object_addhandler( claro, "mainloop", claro_net_loop );
}

/* claro.net loop itteration */
event_handler( claro_net_loop )
{
	node_t *n, *nn;
	socket_t *sock;
	fd_set fdRead, fdWrite, fdExcept;
	struct timeval m;
	int a;
	
	FD_ZERO( &fdRead );
	FD_ZERO( &fdWrite );
	FD_ZERO( &fdExcept );
	
	LIST_FOREACH( n, claro_net->sockets.head )
	{
		sock = (socket_t *)n->data;
		
		if ( sock->status == cSocketDisconnected || sock->status == cSocketError )
			continue; /* nothing to do here. */
		
		FD_SET( sock->s, &fdRead );
		FD_SET( sock->s, &fdWrite );
		FD_SET( sock->s, &fdExcept );
	}
	
	m.tv_sec = cSocketSelectTime / 1000;
	m.tv_usec = cSocketSelectTime % 1000;
	
	a = select( claro_net->maxfd + 1, &fdRead, &fdWrite, &fdExcept, &m );
	
	if ( a == 0 ) /* nothing has changed */
		return;
	
	LIST_FOREACH_SAFE( n, nn, claro_net->sockets.head )
	{
		sock = (socket_t *)n->data;
		
		if ( sock->status == cSocketDisconnected || sock->status == cSocketError )
			continue; /* nothing to do here. */
		
		if ( FD_ISSET( sock->s, &fdExcept ) )
		{
			/* an error has occured! */
			sock->error = cSocketErrorClosed;
			socket_set_state( OBJECT(sock), cSocketError );
			socket_set_state( OBJECT(sock), cSocketDisconnected );
			socket_close( OBJECT(sock) );
			continue;
		}
		
		if ( sock->status == cSocketListening )
		{
			if ( FD_ISSET( sock->s, &fdRead ) )
			{
				socket_set_state( OBJECT(sock), cSocketClientWaiting );
			}
			
			continue;
		}
		
		if ( sock->status == cSocketConnecting && FD_ISSET( sock->s, &fdWrite ) )
		{
			socket_set_state( OBJECT(sock), cSocketConnected );
		}
		
		if ( FD_ISSET( sock->s, &fdRead ) )
		{
			int num_recv = 0;
			int chars;
			char buf[1024];
			
			while ( ( chars = recv( sock->s, buf, 1024, 0 ) ) > 0 )
			{
				num_recv++;
				/*
				for ( b = 0; b < a; b++ )
					if ( bb[b] == '\n' )
						c++;
				
				b = curr->read_size;
				
				curr->read_size += a;
				curr->read_buffer = (char *)realloc( curr->read_buffer, curr->read_size + 1 );
				
				memcpy( &curr->read_buffer[b], &bb, a );
				curr->read_buffer[curr->read_size] = 0;
				*/
			}
			
			if ( num_recv == 0 )
			{
				/* we got NO data! disconnected. */
				sock->error = cSocketErrorClosed;
				socket_set_state( OBJECT(sock), cSocketError );
				socket_set_state( OBJECT(sock), cSocketDisconnected );
				socket_close( OBJECT(sock) );
				continue;
			}
			
			/*curr->read_lines += c;*/
		}
	}
}

void socket_close( object_t *sock )
{
	object_destroy( sock );
}

int socket_error( object_t *sock )
{
	return ((socket_t *)sock)->error;
}

object_t *socket_create( object_t *parent, int flags )
{
	socket_t *sock;
	node_t *n;
	
	sock = (socket_t *)object_create( parent, sizeof(socket_t), "claro.net.socket" );
	
	n = node_create( );
	node_add( sock, n, &claro_net->sockets );
	
	return OBJECT(sock);
}

void socket_set_state( object_t *sock, int state )
{
	socket_t *socks = (socket_t *)sock;
	
	socks->status = state;
	
	event_send( sock, cSocketEvents[state], "" );
}

void socket_connect( object_t *sock_obj, char *hostname, int port )
{
	socket_t *sock = (socket_t *)sock_obj;
#ifdef _WIN32
	sock->hostEntry = gethostbyname( hostname );

	socket_set_state( sock, cSocketDisconnected );

	if ( !sock->hostEntry )
	{
		socket_set_state( sock_obj, cSocketError );
		sock->error = cSocketErrorHostnameLookup;
		return;
	}

	sock->s = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

	if ( sock->s == INVALID_SOCKET ) {
		socket_set_state( sock_obj, cSocketError );
		sock->error = cSocketErrorSocketCreate;
		return;
	}

	sock->serverInfo.sin_family = AF_INET;
	sock->serverInfo.sin_addr = *((LPIN_ADDR)*sock->hostEntry->h_addr_list);
	sock->serverInfo.sin_port = htons( port );

	a = 1;
	if( ioctlsock_obj( sock->s, FIONBIO, (unsigned long *)&a ) == -1 ) {
		socket_set_state( sock_obj, cSocketError );
		sock->error = cSocketErrorSocketParameters;
		return;
	}
	
	connect( sock->s, (LPSOCKADDR)&sock->serverInfo, sizeof( struct sockaddr ) );
#else
	sock->server = gethostbyname( hostname );
	
	if ( !sock->server )
	{
		socket_set_state( sock_obj, cSocketError );
		sock->error = cSocketErrorHostnameLookup;
		return;
	}
	
	sock->s = socket( AF_INET, SOCK_STREAM, 0 );
	
	if ( sock->s < 0 ) {
		socket_set_state( sock_obj, cSocketError );
		sock->error = cSocketErrorSocketCreate;
		return;
	}
	
	memset( &sock->serv_addr, 0, sizeof( sock->serv_addr ) );
	sock->serv_addr.sin_family = AF_INET;
	bcopy( (char *)sock->server->h_addr, (char *)&sock->serv_addr.sin_addr.s_addr, sock->server->h_length );
	sock->serv_addr.sin_port = htons( port );
	
	if ( fcntl( sock->s, F_SETFL, O_NONBLOCK ) == -1 ) {
		socket_set_state( sock_obj, cSocketError );
		sock->error = cSocketErrorSocketParameters;
		return;
	}
	
	connect( sock->s, (struct sockaddr *)&sock->serv_addr, sizeof( sock->serv_addr ) );
#endif
	
	socket_set_state( sock_obj, cSocketConnecting );
}

void socket_listen( object_t *sock_obj, char *hostname, int port )
{
	socket_t *sock = (socket_t *)sock_obj;
#ifdef _WIN32
	
#else
	struct in_addr *in;
	int optval;
	
	sock->server = gethostbyname( hostname );
	
	if ( !sock->server )
	{
		clog( CL_WARNING, "Hostname lookup failed: '%s'", hostname );
		socket_set_state( sock_obj, cSocketError );
		sock->error = cSocketErrorHostnameLookup;
		return;
	}
	
	sock->s = socket( AF_INET, SOCK_STREAM, 0 );
	
	if ( sock->s < 0 ) {
		clog( CL_WARNING, "Socket creation failed." );
		socket_set_state( sock_obj, cSocketError );
		sock->error = cSocketErrorSocketCreate;
		return;
	}
	
	/* Has the highest fd gotten any higher yet? */
	if ( sock->s > claro_net->maxfd )
		claro_net->maxfd = sock->s;
	
	memset( &sock->serv_addr, 0, sizeof( sock->serv_addr ) );
	sock->serv_addr.sin_family = AF_INET;
	
	in = (struct in_addr *)(sock->server->h_addr_list[0]);
	sock->serv_addr.sin_family = AF_INET;
	sock->serv_addr.sin_addr.s_addr = in->s_addr;
	sock->serv_addr.sin_port = htons( port );

	optval = 1;
	setsockopt( sock->s, SOL_SOCKET, SO_REUSEADDR, (char *)&optval, sizeof(optval) );

	if ( bind( sock->s, (struct sockaddr *)&sock->serv_addr, sizeof(sock->serv_addr) ) < 0 )
	{
		clog( CL_WARNING, "Bind failed on socket." );
		close( sock->s );
		socket_set_state( sock_obj, cSocketError );
		sock->error = cSocketErrorBindFailed;
		return;
	}

	if ( fcntl( sock->s, F_SETFL, O_NONBLOCK ) == -1 ) {
		clog( CL_WARNING, "Could not set socket to non-blocking." );
		close( sock->s );
		socket_set_state( sock_obj, cSocketError );
		sock->error = cSocketErrorSocketParameters;
		return;
	}
	
	if ( listen( sock->s, 5 ) < 0 )
	{
		clog( CL_WARNING, "Could not listen on socket." );
		close( sock->s );
		socket_set_state( sock_obj, cSocketError );
		sock->error = cSocketErrorListenFailed;
		return;
	}
#endif
	
	socket_set_state( sock_obj, cSocketListening );
}

object_t *socket_accept( object_t *server )
{
	struct sockaddr sa;
	socket_t *sock = (socket_t *)server;
	socket_t *client = NULL;
	socklen_t addr_len;
	int cs;
	
	if ( ((socket_t *)server)->status != cSocketClientWaiting )
		return NULL;
	
	socket_set_state( server, cSocketListening );
	
	addr_len = sizeof(sa);
	cs = accept( sock->s, &sa, &addr_len );
	
	if ( cs == -1 ) {
		return NULL;
	}
	
	client = (socket_t *)socket_create( OBJECT(claro_net), 0 );
	client->s = cs;
	
	if ( fcntl( client->s, F_SETFL, O_NONBLOCK ) == -1 ) {
		clog( CL_WARNING, "Could not set socket to non-blocking." );
		close( client->s );
		socket_set_state( OBJECT(client), cSocketError );
		client->error = cSocketErrorSocketParameters;
		return OBJECT(client);
	}
	
	/* Has the highest fd gotten any higher yet? */
	if ( client->s > claro_net->maxfd )
		claro_net->maxfd = client->s;
	
	memcpy( &client->client_addr, &sa, sizeof(sa) );
	
	socket_set_state( OBJECT(client), cSocketConnecting );
	
	return OBJECT(client);
}

