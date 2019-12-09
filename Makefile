TARGET = ppfinal
SRC = ppfinal_openmp.cpp
CC = g++
CCFLAG = -Wall -lm -O2 -fopenmp

all: $(TARGET)

ppfinal:
	$(CC) -o $(TARGET) $(SRC) $(CCFLAG)
