OBJS = main.o decisionTree.o
CC = g++
DEBUG = -g -O3
CFLAGS = -Wall -fPIC -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

runProg : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o runProg

main.o : main.cpp decisionTree.cpp decisionTree.h
	$(CC) $(CFLAGS) main.cpp

decisionTree.o : decisionTree.cpp decisionTree.h
	$(CC) $(CFLAGS) decisionTree.cpp

clean:	
	\rm *.o runProg
