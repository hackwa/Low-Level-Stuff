INC = -I .
FLAGS = -Wall -g

all:
	g++ $(FLAGS) $(INC) -c tokenizer.cc -o tok.o
	g++ $(FLAGS) $(INC) -c main.cc -o main.o
	g++ tok.o main.o -o compile