TARGET = ppfinal
SRC = ppfinal_openmp.c
CC = gcc
CCFLAG = -Wall -lm -O2

all: $(TARGET)

ppfinal:
        $(CC) -o $(TARGET) $(SRC) $(CCFLAG)
