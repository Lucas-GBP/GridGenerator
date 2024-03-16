##
## Variables
##
CC = gcc
SRC_DIR = src
BUILD_DIR = build
WARNINGS =-Wall -Wextra -Wshadow -Wconversion
C_FLAGS = -O2 $(WARNINGS)

##
## OS Variables
##
ifeq '$(findstring ;,$(PATH))' ';'
  OS := PlainWindows
else
  OS := $(shell uname 2>/dev/null || echo Unknown)
  OS := $(patsubst CYGWIN%,Cygwin,$(OS))
  OS := $(patsubst MSYS%,MSYS,$(OS))
  OS := $(patsubst MINGW%,MINGW,$(OS))
endif

##
## Commands and Files
##
ifeq ($(OS), PlainWindows)
  TARGET = executable.exe
  RM_COMMAND = del /q /f
  CLEAN_PATH = *.o
  SOURCE_FILES = src\main.c #Por o diretorio de todos os arquivos .c
  O_FILES = $(subst src\,build\,$(SOURCE_FILES:.c=.o))
else
  TARGET = executable.out
  RM_COMMAND = rm -f
  MAKE_DIR = mkdir -p
  SOURCE_FILES = $(shell find $(SRC_DIR) -type f -name \*.c)
  O_FILES = $(patsubst $(SRC_DIR)/%,$(BUILD_DIR)/%,$(SOURCE_FILES:.c=.o))
endif

##
## Targets
##
$(TARGET): $(O_FILES)
	@echo compiling executable...
	@$(CC) $^ -o $(TARGET) $(C_FLAGS)

## BUILD for linux
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo compiling $@
	@$(CC) $(C_FLAGS) -c -o $@ $<

## BUILD for windows
$(BUILD_DIR)\\%.o: $(SRC_DIR)\%.c
	@if not exist $(dir $@) mkdir $(dir $@)
	@echo compiling $@
	@$(CC) $(C_FLAGS) -c -o $@ $<

##
## Commands
##
build: $(TARGET)

run:
	@echo running $(TARGET)
	@./$(TARGET) 25 25

clean:
	@echo deleting... $(O_FILES)
	@$(RM_COMMAND) $(O_FILES) $(TARGET)
	@clear

test:
	@echo OS: $(OS)
	@echo SOURCE_FILES: $(SOURCE_FILES)
	@echo O_FILES: $(O_FILES)
	@echo TARGET: $(TARGET)

.PHONY: clean build run