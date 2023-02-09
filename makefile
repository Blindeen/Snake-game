COMP = g++
EXT = cpp

CFLAGS = -Wall -pedantic -std=c++11
LDFLAGS = -lncurses

SRC_DIR = ./src
OBJ_DIR = ./obj

SRC = $(wildcard $(SRC_DIR)/*.$(EXT))
OBJ = $(patsubst $(SRC_DIR)/%.$(EXT), $(OBJ_DIR)/%.o, $(SRC))
EXEC = Snake

all: $(OBJ_DIR) $(EXEC)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(EXEC): $(OBJ)
	$(COMP) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.$(EXT)
	$(COMP) $(CFLAGS) -o $@ -c $<

.PHONY: clean run

clean:
	rm -rf $(OBJ_DIR) $(EXEC)

run: $(OBJ_DIR) $(EXEC)
	./$(EXEC)
