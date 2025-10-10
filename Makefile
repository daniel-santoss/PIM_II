CC=gcc
CFLAGS=-Wall -Wextra -std=c99
TARGET=sistema_escolar
SOURCE=sistema_escolar.c

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

clean:
	del $(TARGET).exe *.txt

run: $(TARGET)
	./$(TARGET)

.PHONY: clean run