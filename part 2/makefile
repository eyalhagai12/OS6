BINARYS= guard singleton reactor client

all: $(BINARYS)

guard: Guard.o
	g++ -o $@ $^ -lpthread

singleton: singleton_main.o
	rm -f test.txt
	touch test.txt
	g++ -o $@ $^ -lpthread

reactor: Reactor.o
	g++ -o reactor $^ -lpthread

client: client.o
	g++ -o $@ $^ -lpthread


%.o: %.cpp %(HEADERS)
	g++ --compile -g $< 

clean:
	rm -f *.o $(BINARYS) *.txt