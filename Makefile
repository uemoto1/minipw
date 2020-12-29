include make.inc

minipw: main.c
	$(CC) $^ -o $@ $(LIBS) $(INCLUDES) $(FLAGS)
