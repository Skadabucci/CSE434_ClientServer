#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <algorithm>
#include <unistd.h>
// This to be relplaced with sys/socket.h
#include <sys/socket.h>
//------------------------------------------
#include "Request.h"

#define MAX_LENGTH 5

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
	if (savedString.length > 5)
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
	request *newRequest;				/* Request received from client */
	vector<clientEntry> clientTable;/* The client table of all the clients */
	clientEntry newClientEntry;		/* Client for client table lookup */
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
        if ((recvMsgSize = recvfrom(sock, (void*)newRequest, sizeof(newRequest), 0,
			(struct sockaddr *) &clientAddr, (int*) &clientAddrLength)) < 0)
            DieWithError("recvfrom() failed");

        printf("Handling client %s\n", inet_ntoa(clientAddr.sin_addr));

		/* Lookup the client in the client talbe */
		auto it = find_if(clientTable.begin(), clientTable.end(), find_id(newRequest->client));

		/* If the client is not found, add it */
		if (it == clientTable.end()) {
			clientTable.push_back(Client(getpid(), newRequest->inc, newRequest->client, 0));
		}
		/* Else update the existing entry */
		else {
			it->process_id = getpid();
			it->inc = newRequest->inc;
			it->client = newRequest->client;
		}

		/* These cases compare 'R' (client request number) to 'r' (server request number) */
		if (newRequest->req < it->requestNum) {
			printf("The request from %d was ignored.\n", clientAddr.sin_addr);
			printf("The client's request number (%d) was less than the server's request number (%d)\n\n", newRequest->req, it->requestNum);
		}
		else if (newRequest->req == it->requestNum) {
			/* Send received datagram back to the client */
			if (sendto(sock, (void*)&it->sendMsg, recvMsgSize, 0, 
				 (struct sockaddr *) &clientAddr, sizeof(clientAddr)) != recvMsgSize)
				DieWithError("sendto() sent a different number of bytes than expected");
			printf("The client's request number (%d) was equal to the server's request number (%d)\n\n", newRequest->req, it->requestNum);
		}
		else if (newRequest->req > it->requestNum) {
			/* Modify the message to send back */
			it->sendMsg = ModifyString(it->sendMsg, newRequest->c);
			/* Send received datagram back to the client */
			if (sendto(sock, (void*)&it->sendMsg, recvMsgSize, 0, 
				 (struct sockaddr *) &clientAddr, sizeof(clientAddr)) != recvMsgSize)
				DieWithError("sendto() sent a different number of bytes than expected");
			printf("The client's request number (%d) was greater than the server's request number (%d)\n\n", newRequest->req, it->requestNum);
		}

		sort(clientTable.begin(), clientTable.end(), compareByClient);
    }
    /* NOT REACHED */
	return 0;
}