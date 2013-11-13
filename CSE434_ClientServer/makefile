OBJS = Client.o Server.o
CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall -c $(DEBUG)

p1 : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o p1

Client.o : Request.h
	$(CC) $(CFLAGS) Client.cpp

Server.o : Request.h
	$(CC) $(CFLAGS) Server.cpp

clean:
	\rm *.o *~ p1

