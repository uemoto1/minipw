include make.inc

SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

minipw: $(OBJS)
	$(CC) -o $@ $(OBJS) $(FLAGS) $(LIBS)

.c.o:
	$(CC) -c $< $(FLAGS) $(INCLUDES)

.PHONY: all clean

all: minipw

clean:
	rm -f minipw $(OBJS)
