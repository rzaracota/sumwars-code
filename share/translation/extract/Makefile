# executable file for project
BIN=extract

# flags (c-compiler)
CFLAGS=-Wall

# flags (c++-compiler)
CXXFLAGS=-Wall -std=c++0x

# libs (linker)
LDLIBS=-lPocoFoundation -lPocoXML

OBJ=extract.o

all: default

default: link

link: compile
	$(CXX) $(CXXFLAGS) -o $(BIN) $(OBJ) $(LDLIBS)
	
compile: $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<
	
run: link
	./$(BIN)

clean:
	rm -f *.o $(BIN)


