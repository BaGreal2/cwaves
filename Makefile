CXX = cc

OUT = waves
SRC = $(wildcard src/*.c)

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
	CXXFLAGS = -Wall -Wextra -O3 -I./libs/raylib-5.5_macos/include -I./include
	LFLAGS = -L./libs/raylib-5.5_macos/lib -lraylib -lm -lpthread  -framework OpenGL -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
else
	ifeq ($(UNAME_S),Linux)
    LFLAGS = -L./libs/raylib-5.5_linux_amd64/lib -l:libraylib.a -lm -lpthread -lGL -ldl -lrt -lX11
    CXXFLAGS = -Wall -Wextra -O3 -I./libs/raylib-5.5_linux_amd64/include -I./include
	else
		$(error Unsupported OS: $(UNAME_S))
	endif
endif

all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LFLAGS)

clean:
	rm -f $(OUT)

.PHONY: all clean
