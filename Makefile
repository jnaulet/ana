TARGET=ana
OBJ=ana.o


all:$(TARGET)

$(TARGET): $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

%.o:%.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -f $(OBJ) $(TARGET)
