EXE = process.out
CC = g++
SDIR = src
ODIR = obj

SRC = $(wildcard $(SDIR)/*.cpp)

OBJ = $(SRC:$(SDIR)/%.cpp=$(ODIR)/%.o)

CFLAGS += `libpng-config --cflags`
CFLAGS += -Wall -g

LDFLAGS += `libpng-config --ldflags`

all: $(EXE)

$(EXE) : $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

$(ODIR)/%.o: $(SDIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)

.PHONY: all clean