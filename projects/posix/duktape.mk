DuktapeObjects = duktape.o

duktape.o: $(BkRoot)src/duktape/duktape.c
	$(CC) -c $(CCFLAGS) -I$(BkRoot)src/duktape $< -o $@
