include make.inc

minipw: main.o linear.o
	$(CC) $^ -o $@ $(FLAGS) $(LIBS)

main.o: main.c common.h
	$(CC) $< -c -o $@ $(FLAGS) $(INCLUDES)

linear.o: linear.c linear.h common.h
	$(CC) $< -c -o $@ $(FLAGS) $(INCLUDES)

.PHONY: all clean

all: minipw

clean:
	rm -f minipw *.o
