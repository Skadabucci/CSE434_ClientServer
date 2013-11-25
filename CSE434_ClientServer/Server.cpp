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

<<<<<<< HEAD
string ModifyString(string savedString, char toAppend)
{
	savedString = toAppend + savedString;
	if (savedString.length() > 5)
		savedString  = savedString.substr(0, 4);
	return savedString;
}

=======
>>>>>>> 5c7045fd19f47c9366183201a1c64f0fddb76988
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
<<<<<<< HEAD
        /* Set the size of the in-out parameter */
        clientAddrLength = sizeof(clientAddr);

        /* Block until receive message from a client */
        if ((recvMsgSize = recvfrom(sock, echoBuffer, sizeof(MAX_LENGTH), 0,
=======
		newClientEntry = (clientEntry*)malloc(sizeof(clientEntry));

        /* Set the size of the in-out parameter */
        clientAddrLength = sizeof(clientAddr);
		memset(&echoBuffer, 0, sizeof(echoBuffer));
        /* Block until receive message from a client */
        if ((recvMsgSize = recvfrom(sock, echoBuffer, MAX_LENGTH, 0,
>>>>>>> 5c7045fd19f47c9366183201a1c64f0fddb76988
			(struct sockaddr *) &clientAddr, &clientAddrLength)) < 0)
            DieWithError("recvfrom() failed");

        printf("Handling client %s\n", inet_ntoa(clientAddr.sin_addr));
<<<<<<< HEAD

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
=======
		
		//const unsigned char * const px = (unsigned char*)&echoBuffer;
		/*
		unsigned int i;
		cout << "HEX DUMPING" << endl;
		for (i = 0; i < sizeof(echoBuffer); ++i) printf("%02X ", px[i]);
		cout << "DONE" << endl;
		*/
		
		/* Parsing buffer to newRequest */
		memset(&newRequest, 0, sizeof(request));
		memcpy(&newRequest, &echoBuffer, sizeof(request));
		/* Lookup the client in the client talbe */
		vector<clientEntry>::iterator iter = find_if(clientTable.begin(), clientTable.end(), find_id(newRequest.client));

		/* If the client is not found, add it */
		if (iter == clientTable.end()) {
			//printf("inside if\n");
			newClientEntry->process_id = getpid();
			//printf("get pid\n");

			newClientEntry->inc = newRequest.inc;
			//printf("inc\n");

			newClientEntry->client = newRequest.client;
			//printf("client\n");
			
			newClientEntry->requestNum = newRequest.req;
			//printf("reqd\n");

			strcpy(newClientEntry->sendMsg, "     ");			
			//printf("dddd\n");
			
			clientTable.push_back(*newClientEntry);
			//printf("push back\n");
			
			iter = clientTable.end()-1;
			//printf("end-1\n");
			PrintRequest(&newRequest);
		}

>>>>>>> 5c7045fd19f47c9366183201a1c64f0fddb76988
		/* Else the iterator is pointing to the discovered client */

		/* These cases compare 'R' (client request number) to 'r' (server request number) */
		if (newRequest.req < iter->requestNum) {
<<<<<<< HEAD
=======
			printf("\ncase 1 \n");
			
>>>>>>> 5c7045fd19f47c9366183201a1c64f0fddb76988
			printf("The request from %d was ignored.\n", clientAddr.sin_addr);
			printf("The client's request number (%d) was less than the server's request number (%d)\n\n", newRequest.req, iter->requestNum);
			++iter->requestNum;
		}
		else if (newRequest.req == iter->requestNum) {
			/* Send received datagram back to the client */
<<<<<<< HEAD
			if (sendto(sock, (void*)&iter->sendMsg, recvMsgSize, 0, 
				 (struct sockaddr *) &clientAddr, sizeof(clientAddr)) != recvMsgSize)
=======
			printf("\ncase 2 \n");
			iter->sendMsg[4] = iter->sendMsg[3];
			iter->sendMsg[3] = iter->sendMsg[2];
			iter->sendMsg[2] = iter->sendMsg[1];
			iter->sendMsg[1] = iter->sendMsg[0];
			iter->sendMsg[0] = newRequest.c;
			unsigned char buffer[sizeof(iter->sendMsg)];
			memcpy(&buffer, &iter->sendMsg, sizeof(iter->sendMsg));
			cout << buffer << endl;
			int size = sendto(sock, buffer, sizeof(buffer), 0, 
					(struct sockaddr *) &clientAddr, sizeof(clientAddr));
			cout << size << endl;
			if(size == -1)
>>>>>>> 5c7045fd19f47c9366183201a1c64f0fddb76988
				DieWithError("sendto() sent a different number of bytes than expected");
			printf("The client's request number (%d) was equal to the server's request number (%d)\n\n", newRequest.req, iter->requestNum);
			++iter->requestNum;
		}
		else if (newRequest.req > iter->requestNum) {
<<<<<<< HEAD
			/* Modify the message to send back */
			iter->sendMsg = ModifyString(iter->sendMsg, newRequest.c);
			/* Send received datagram back to the client */
			if (sendto(sock, (void*)&iter->sendMsg, recvMsgSize, 0, 
				 (struct sockaddr *) &clientAddr, sizeof(clientAddr)) != recvMsgSize)
=======
			printf("\ncase 3 \n");

			/* Modify the message to send back */			
			iter->sendMsg[4] = iter->sendMsg[3]; //toAppend + iter->sendMsg;
			iter->sendMsg[3] = iter->sendMsg[2];
			iter->sendMsg[2] = iter->sendMsg[1];
			iter->sendMsg[1] = iter->sendMsg[0];
			iter->sendMsg[0] = newRequest.c;
			/* Send received datagram back to the client */
			unsigned char buffer[sizeof(iter->sendMsg)];
			memcpy(&buffer, &iter->sendMsg, sizeof(iter->sendMsg));
			
			if (sendto(sock, buffer, sizeof(buffer), 0, 
					(struct sockaddr *) &clientAddr, sizeof(clientAddr)) != recvMsgSize)
>>>>>>> 5c7045fd19f47c9366183201a1c64f0fddb76988
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
