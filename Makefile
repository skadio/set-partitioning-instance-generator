OBJS = scpGen.o 
CC = g++
DEBUG = -g

CFLAGS = -Wall -Wno-deprecated -DNDEBUG $(DEBUG)
LFLAGS = -Wall $(DEBUG)
NAME = scpGen

$(NAME) : $(OBJS)
	$(CC) -o $(NAME) $(OBJS) $(LFLAGS)

scpGen.o : scpGen.c 
	$(CC) $(CFLAGS) -c scpGen.c

clean:
	rm *.o 
	rm $(NAME)

