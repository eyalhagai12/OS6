all: client server

server: Queue.o ActiveObject.h server.o 
	gcc $^ -o $@ -lpthread

client: Queue.o ActiveObject.h client.o
	gcc $^ -o $@ -lpthread


%.o: %.c 
	gcc -g -c $< -lpthread

clean:
	rm -f *.o main server client