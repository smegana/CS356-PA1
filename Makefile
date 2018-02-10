CC = g++
CFLAGS = -Wall -c -g
LFLAGS = -Wall
OBJS = PA1.o
INCLUDE = -I.

PA1 : $(OBJS)
	$(CC) $(OBJS) $(LFLAGS) $(INCLUDE) -o PA1

PA2.o : PA1.cpp PA1.h
	$(CC) PA1.cpp $(INCLUDE) $(CFLAGS)

clean :
	-rm *.o PA1

