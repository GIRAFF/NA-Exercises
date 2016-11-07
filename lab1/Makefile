CC=g++
TARGET=src/*.cc
FLAGS=-std=c++11

all:
	$(CC) $(TARGET) $(FLAGS) -g -D DEBUG
float:
	$(CC) $(TARGET) $(FLAGS) -o float.out
double:
	$(CC) $(TARGET) $(FLAGS) -D USE_DOUBLE -o double.out
scal:
	$(CC) $(TARGET) $(FLAGS) -D DOUBLE_SCAL -o scal.out
test:
	./test.sh > test.tex
	pdflatex test.tex
