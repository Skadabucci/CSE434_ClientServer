#ifndef __CLIENT_REQUEST__

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <string.h>
#pragma pack(1)
typedef struct requestf 
{
	char client_ip[16];		/* To hold client IP address in dotted decimal */
	int inc;				/* Incarnation number of client */
	int client;				/* Client number */
	int req;				/* Request number */
	char c;					/* Random character client sends to server */
} request;
#pragma pack(0)

request Request(std::string clientip, int clientNumber, int inc, int req, char c) 
{
	request r;// =(request*)malloc(sizeof(request));	
	memset(&r, 0, sizeof(request));
	r.inc = inc;
	r.client = clientNumber;
	r.req = req;
	int ipSize = clientip.size();
	for (int i = 0 ; i < ipSize; i++)
	{
		r.client_ip[i] = clientip[i];
	}
	r.client_ip[16] = '\0';
	r.c = c;

	return r;
}

void PrintRequest(request* r)
{
	std::cout << "request#: " << r->req << std::endl;
	std::cout << "inc: " << r->inc << std::endl;
	std::cout << "client#: " << r->client << std::endl;
	std::cout << "char: " << r->c << std::endl;
	std::cout << "client_ip: " << r->client_ip << std::endl;
}

typedef struct clientf {
        int process_id;                        /* Process ID for client table */
        int inc;                               /* Incarnation number of client */
        int client;                            /* Client number */
        int requestNum;                        /* Request number 'r' */
        char sendMsg[5];        			   /* The string saved for the client */
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
