#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "Request.h"

char charToSend() {
	char randChar = ' ';
	int randNum = 0;
	randNum = 26 * (rand() / (RAND_MAX + 1.0));
	randNum += 97;
	randChar = (char)randNum;
}

int locking() 
{

	int fd = -1;

	if ((fd = open("inc.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR)) == -1) 
	{
		/*
		int myerr = errno;
		printf("ERROR: open errno(%d): %s\n", errno, strerror(myerr));
		return EXIT_FAILURE;
		*/
	}

	/*
	* Take a write lock on the file and do the write.
	*/
	/*
	if (WriteLock(fd) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}

	if (write(fd, (const void *)writestr, strlen(writestr) + 1) == -1) 
	{
		int errno = errno;
		printf("ERROR: write errno(%d): %s\n", errno, strerror(myerr));
		return EXIT_FAILURE;
	}

	if (UnLock(fd) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}
	*/
}

int main(int argc, const char* argv[]) {
	//FILE *fp;
	//fp = fopen("inc.txt", "r");

	//struct requestf newRequest = Requestf(/*Put parameters here*/);
	struct requestf newRequest;

	while (1) 
	{
		newRequest.c = charToSend();
		printf("%c\n", newRequest.c);
	}

	return 0;
}