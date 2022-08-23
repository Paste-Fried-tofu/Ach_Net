CC = g++
INCLUDES = -Iinclude
CFLAG = -g -Wall -std=c++2a -DDBUG -O3 -mavx -mfma

SRC = example.cpp

OBJ = $(SRC:.cpp = .o)

EXE = example

.PHONY:clean

EXE: $(OBJ)
	$(CC) $(CFLAG) $(INCLUDES) $(OBJ) -o $(EXE)
%.cpp: %.o
	$(CC) $(CFLAG) $(INCLUDES) -c $< -o $@
clean:
	rm $(OBJ)
