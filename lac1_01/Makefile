# Declaration of variables
CC = g++
CC_FLAGS = -Ofast  -W -Wall -pedantic -lm -std=c++0x -I header/  
#CC_FLAGS = -O0 -pg  -W -Wall -pedantic -lm -std=c++0x -I header/  

# File names
EXEC = lazy
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

# Main target
$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC)

# To obtain object files
%.o: %.cpp
	$(CC) -c $(CC_FLAGS) $< -o $@

run: $(EXEC)
	./$(EXEC) -i input/trainning.txt -t input/test.txt -m 3 -c 0.001 -s 1 -e 500000 
	
help: $(EXEC)
	./$(EXEC) -h
	
clean:
	rm -f $(EXEC) $(OBJECTS)
