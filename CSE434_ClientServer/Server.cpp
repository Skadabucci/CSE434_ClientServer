#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <arpa/inet.h>
// This to be relplaced with sys/socket.h
#include <sys/socket.h>
//------------------------------------------
#include "Request.h"

#define MAX_LENGTH 255

using namespace std;

struct find_id : std::unary_function<clientEntry, bool> {
    int client;
    find_id(int client):client(client) { }
    bool operator()(clientEntry const& myEntry) const {
        return myEntry.client == client;
    }
};

void DieWithError(const char *errorMessage) /* External error handling function */
{
    perror(errorMessage);
    exit(EXIT_FAILURE);
}

string ModifyString(string savedString, char toAppend)
{
	savedString = toAppend + savedString;
	if (savedString.length() > 5)
		savedString  = savedString.substr(0, 4);
	return savedString;
}

bool compareByClient(const clientEntry &a, const clientEntry &b)
{
	return a.client < b.client;
}

int main(int argc, char* argv[])
{
	int sock;                       /* Socket */
    struct sockaddr_in serverAddr;	/* Local address */
    struct sockaddr_in clientAddr;	/* Client address */
    unsigned int clientAddrLength;  /* Length of incoming message */
	request newRequest;			/* Request received from client */
	vector<clientEntry> clientTable;/* The client table of all the clients */
	clientEntry *newClientEntry;	/* Client for client table lookup */
	char echoBuffer[MAX_LENGTH];	/* Raw data to receive from the client */
    unsigned short serverPort;		/* Server port */
    int recvMsgSize;                /* Size of received message */
	
	if(argc != 2)
		DieWithError("Incorrect number of arguments.");

	serverPort = atoi(argv[1]);

	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
		DieWithError("socket() failed");

	/* Construct local address structure */
    memset(&serverAddr, 0, sizeof(serverAddr));		/* Zero out structure */
    serverAddr.sin_family = AF_INET;                /* Internet address family */
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    serverAddr.sin_port = htons(serverPort);		/* Local port */

	/* Bind to the local address */
    if (bind(sock, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0)
        DieWithError("bind() failed");

	for (;;) /* Run forever */
    {
        /* Set the size of the in-out parameter */
        clientAddrLength = sizeof(clientAddr);

        /* Block until receive message from a client */
        if ((recvMsgSize = recvfrom(sock, echoBuffer, sizeof(MAX_LENGTH), 0,
			(struct sockaddr *) &clientAddr, &clientAddrLength)) < 0)
            DieWithError("recvfrom() failed");

        printf("Handling client %s\n", inet_ntoa(clientAddr.sin_addr));

		/* Parsing buffer to newRequest */
		memcpy(&newRequest, &echoBuffer, sizeof(request));

		/* Lookup the client in the client talbe */
		auto iter = find_if(clientTable.begin(), clientTable.end(), find_id(newRequest.client));

		/* If the client is not found, add it */
		if (iter == clientTable.end()) {
			newClientEntry->process_id = getpid();
			newClientEntry->inc = newRequest.inc;
			newClientEntry->client = newRequest.client;
			newClientEntry->requestNum = newRequest.req;
			newClientEntry->sendMsg = "";
			clientTable.push_back(*newClientEntry);
			iter = clientTable.end()-1;
		}
		/* Else the iterator is pointing to the discovered client */

		/* These cases compare 'R' (client request number) to 'r' (server request number) */
		if (newRequest.req < iter->requestNum) {
			printf("The request from %d was ignored.\n", clientAddr.sin_addr);
			printf("The client's request number (%d) was less than the server's request number (%d)\n\n", newRequest.req, iter->requestNum);
			++iter->requestNum;
		}
		else if (newRequest.req == iter->requestNum) {
			/* Send received datagram back to the client */
			if (sendto(sock, (void*)&iter->sendMsg, recvMsgSize, 0, 
				 (struct sockaddr *) &clientAddr, sizeof(clientAddr)) != recvMsgSize)
				DieWithError("sendto() sent a different number of bytes than expected");
			printf("The client's request number (%d) was equal to the server's request number (%d)\n\n", newRequest.req, iter->requestNum);
			++iter->requestNum;
		}
		else if (newRequest.req > iter->requestNum) {
			/* Modify the message to send back */
			iter->sendMsg = ModifyString(iter->sendMsg, newRequest.c);
			/* Send received datagram back to the client */
			if (sendto(sock, (void*)&iter->sendMsg, recvMsgSize, 0, 
				 (struct sockaddr *) &clientAddr, sizeof(clientAddr)) != recvMsgSize)
				DieWithError("sendto() sent a different number of bytes than expected");
			printf("The client's request number (%d) was greater than the server's request number (%d)\n\n", newRequest.req, iter->requestNum);
			++iter->requestNum;
		}

		/* Sort the client table in the interest of lookup times */
		sort(clientTable.begin(), clientTable.end(), compareByClient);
    }
    /* NOT REACHED */
	return 0;
}