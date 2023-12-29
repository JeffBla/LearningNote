CC := g++
CFLAGS := -Wall
TARGET := final1
v := 1

all: $(TARGET)

$(TARGET):f74101220_final1.cpp
	$(CC) -o $@ $^ -fopenmp

judge: all
	@judge -v ${v} || printf "or \`make judge v=1\`"

clean:
	rm -f $(TARGET)
