#ifndef __CLIENT_REQUEST__

#include <stdio.h>
#include <stdlib.h>
#include <string>

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

	return newRequest;
}

typedef struct clientf {
	int process_id;			/* Process ID for client table */
	int inc;				/* Incarnation number of client */
	int client;				/* Client number */
	int requestNum;			/* Request number 'r' */
	std::string sendMsg;	/* The string saved for the client */
}clientEntry;

clientEntry* Client(int process_id, int inc, int client, int requestNum) {
	clientEntry* newClientEntry = (clientEntry*)malloc(sizeof(client));
	
	newClientEntry->process_id = process_id;
	newClientEntry->inc = inc;
	newClientEntry->client = client;
	newClientEntry->requestNum = requestNum;

	return newClientEntry;
}

#endif