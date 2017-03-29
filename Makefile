PROG= main
CFLAGS=	-W -Imongoose -pthread -g
LIBS= -lboost_system -lboost_filesystem -lsqlite3 -ldl -L. -lmongoose

all: web main

main:
	g++ config.cpp data.cpp info.cpp main.cpp web.cpp $(LIBS) $(CFLAGS) -o $(PROG)
web:
	$(CC) -c -Wall -Werror -fpic mongoose/mongoose.c -g;
	$(CC) -shared -o libmongoose.so mongoose.o

clean:
	rm -rf main 
	rm -rf web 
	rm -rf libmongoose.so
	rm -rf mongoose.o
