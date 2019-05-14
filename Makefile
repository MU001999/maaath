DIR_SRC = ./src
DIR_OBJ = ./obj
DIR_BIN = ./bin
DIR_TMP = ./tmp

SRC = $(wildcard ${DIR_SRC}/*.cpp)
OBJ = $(patsubst %.cpp, ${DIR_OBJ}/%.o, $(notdir ${SRC}))

TARGET = dse

DEBUG =

BIN_TARGET = $(DIR_BIN)/$(TARGET)

CC = g++
CPPFLAGS = -g $(DEBUG) -Wall -std=c++17 -I${DIR_SRC}

${BIN_TARGET}: ${OBJ} | ${DIR_BIN} ${DIR_TMP}
	${CC} ${OBJ} -o $@ -lstdc++fs -lpthread
	rm -rf ${DIR_OBJ}/main.o

${DIR_OBJ}/%.o: ${DIR_SRC}/%.cpp | ${DIR_OBJ}
	${CC} ${CPPFLAGS}  -c $< -o $@

${DIR_BIN}:
	mkdir $@

${DIR_OBJ}:
	mkdir $@

${DIR_TMP}:
	mkdir $@

test: ${DIR_OBJ}/main.o
	make clean
	make DEBUG="-D _DEBUG"
	cd bin; ./dse
	rm -rf ${DIR_OBJ}/main.o

.PHONY: clean
clean:
	rm -rf ${DIR_OBJ}/*
	rm -rf ${DIR_BIN}/*
	rm -rf ${DIR_TMP}/*