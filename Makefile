DIR_SRC = ./src
DIR_OBJ = ./obj
DIR_BIN = ./bin

SRC = $(wildcard ${DIR_SRC}/*.cpp)
OBJ = $(patsubst %.cpp, ${DIR_OBJ}/%.o, $(notdir ${SRC}))

TARGET = dse

BIN_TARGET = $(DIR_BIN)/$(TARGET)

CC = g++
CPPFLAGS = -g -Wall -std=c++17 -I${DIR_SRC}

${BIN_TARGET}: ${OBJ} | ${DIR_BIN}
	${CC} ${OBJ} -o $@ -lstdc++fs -lpthread

${DIR_OBJ}/%.o: ${DIR_SRC}/%.cpp | ${DIR_OBJ}
	${CC} ${CPPFLAGS} -c $< -o $@

${DIR_BIN}:
	mkdir $@

${DIR_OBJ}:
	mkdir $@

.PHONY: clean
clean:
	rm -rf ${DIR_OBJ}/*
	rm -rf ${DIR_BIN}/*