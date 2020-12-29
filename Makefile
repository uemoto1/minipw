include make.inc

minipw: main.o linear.o
	$(CC) $^ -o $@ $(FLAGS) $(LIBS)

main.o: main.c
	$(CC) $^ -c -o $@ $(FLAGS) $(INCLUDES)

linear.o: linear.c
	$(CC) $^ -c -o $@ $(FLAGS) $(INCLUDES)

.PHONY: all
all: minipw

