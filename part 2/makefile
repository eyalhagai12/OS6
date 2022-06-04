BINARYS= guard singleton reactor client

all: $(BINARYS)

guard: Guard.o
	clang++-9 -o $@ $^ -lpthread

singleton: Singleton.o
	rm -f test.txt
	touch test.txt
	clang++-9 -o $@ $^ -lpthread

reactor: Reactor.o
	g++ -o reactor $^ -lpthread

client: client.o
	clang++-9 -o $@ $^ -lpthread


%.o: %.cpp %(HEADERS)
	clang++-9 --compile -g $< 

clean:
	rm -f *.o $(BINARYS) *.txt