CC = g++  
CFLAGS = -Wall -g  
LDFLAGS = -lm  
PROC_NAME = proc

all: clean *.o ${PROC_NAME}

Switch.o : Switch.h  
	${CC} ${CFLAGS} -c Switch.cpp
Server.o : Server.h  
	${CC} ${CFLAGS} -c Server.cpp  
Link.o : Link.h  
	${CC} ${CFLAGS} -c Link.cpp
Parameter.o : Parameter.h  
	${CC} ${CFLAGS} -c Parameter.cpp  
BCube.o : BCube.h  
	${CC} ${CFLAGS} -c BCube.cpp  
Utils.o : Utils.h  
	${CC} ${CFLAGS} -c Utils.cpp 
${PROC_NAME} : Switch.o Server.o Link.o Parameter.o BCube.o Utils.o Main.o  
	${CC} ${CFLAGS} Switch.o Server.o Link.o Parameter.o BCube.o Utils.o Main.o ${LDFLAGS} -o ${PROC_NAME}   
clean:  
	rm -rf *.o  
	rm -rf ${PROC_NAME}  