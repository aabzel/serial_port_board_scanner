#PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

PATH_PROJECT := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))
#@echo $(error PATH_PROJECT= $(PATH_PROJECT))
SRC_PATH :=  $(dir $(abspath $(dir $$PWD) ))
#@echo $(error SRC_PATH=$(SRC_PATH))

OBJDIR := $(SRC_PATH)/obj
# the compiler to use
CC = clang

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall 

OPT = -DHAS_TCP
OPT += -DDEPLOY_TCP_CLIENT
OPT += -DDEPLOY_SCAN_COM

#files to link:
LFLAGS=-lws2_32 -liphlpapi
  
EXECUTABLE=board_scanner
# the name to use for both the target source file, and the output file:
#TARGET = main

SOURCES = $(SRC_PATH)/main.c 
SOURCES += $(SRC_PATH)/utils.c 
SOURCES += $(SRC_PATH)/algorithms.c 
SOURCES += $(SRC_PATH)/arrays.c  
SOURCES += $(SRC_PATH)/convert.c  
SOURCES += $(SRC_PATH)/float_utils.c  
SOURCES += $(SRC_PATH)/uTests.c   
SOURCES += $(SRC_PATH)/scan_serial_port.c
SOURCES += $(SRC_PATH)/str_ops.c
SOURCES += $(SRC_PATH)/test_str_ops.c    
SOURCES += $(SRC_PATH)/tcp_client.c
SOURCES += $(SRC_PATH)/win_utils.c

OBJECTS =$(SOURCES:.c=.o)

#@echo $(error OBJECTS= $(OBJECTS))

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OPT) $(CFLAGS) $(OBJECTS) $(LFLAGS) -o $(EXECUTABLE).exe

%.o: %.c
	$(CC) $(CFLAGS) $(OPT)  -c $^  
	mv *.o $(SRC_PATH)
