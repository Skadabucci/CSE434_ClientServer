#ifndef __CLIENT_REQUEST__

#include <stdio.h>
#include <stdlib.h>

typedef struct requestf {
	char client_ip[16];		/* To hold client IP address in dotted decimal */
	int inc;				/* Incarnation number of client */
	int client;				/* Client number */
	int req;				/* Request number */
	char c;					/* Random character client sends to server */
} request;

request* Request(char* client_ip, int inc, int client, int req, char c) {
	request* newRequest = (request*)malloc(sizeof(request));
	int i;

	for (i = 0; i < 16; ++i) {
		newRequest->client_ip[i] = *client_ip;
		++client_ip;
	}
	newRequest->inc = inc;
	newRequest->client = client;
	newRequest->req = req;
	newRequest->c = c;
}

#endif