all: main

main: main.o Queue.o
	gcc $^ -o $@


%.o: %.c 
	gcc -g -c $<

clean:
	rm *.o main