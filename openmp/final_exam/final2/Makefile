CC := g++
CFLAGS := -Wall
TARGET := final2
v := 1

all: $(TARGET)

$(TARGET):final2_type3.cpp
	$(CC) -o $@ $^ -fopenmp

judge: all
	@judge -v ${v} || printf "or \`make judge v=1\`"

clean:
	rm -f $(TARGET)
