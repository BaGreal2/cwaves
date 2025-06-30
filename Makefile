CXX = gcc
LFLAGS = -L./raylib-5.5_linux_amd64/lib -l:libraylib.a -lm -lpthread -lGL -ldl -lrt -lX11
CXXFLAGS = -Wall -Wextra -O3 -I./raylib-5.5_linux_amd64/include -I./include

OUT = waves
SRC = $(wildcard src/*.c)

all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LFLAGS)

clean:
	rm -f $(OUT)

.PHONY: all clean
