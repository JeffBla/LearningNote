CFLAGS := -Wall -O0 -ggdb
INCLUDES = -I include/ -I src/
YFLAG := -d -v
MAIN_SRC := ./main.c
LEX_SRC := ./compiler.l
YAC_SRC := ./compiler.y
COMMON := ./compiler_common.h
BUILD := ./build
BUILD_OUT := out
COMPILER := compiler
EXEC := Main
INPUT_CPP := test.cpp

SRC_DIR := src
SRC := $(wildcard ${SRC_DIR}/*.c)
OBJ := $(patsubst %.c,${BUILD}/%.o,$(notdir ${SRC}))

COMPILER_OUT := ${BUILD}/${BUILD_OUT}/${COMPILER}
LEX_OUT := ${BUILD}/lex.yy.c
YAC_OUT := ${BUILD}/y.tab.c
MAIN_OUT := ${BUILD}/main.o
JAVA_ASM_OUT := ${BUILD}/Main.j

all: build run

build: ${COMPILER}

.PHONY: main.c

debug: DEBUG=-DDEBUG
debug: build

create_build_folder:
	mkdir -p ${BUILD}
	mkdir -p ${BUILD}/${BUILD_OUT}

lex.yy.c:
	$(info ---------- Compile Lex ----------)
	lex -o ${LEX_OUT} ${LEX_SRC}

y.tab.c:
	$(info ---------- Compile Yacc ----------)
	yacc ${YFLAG} -o ${YAC_OUT} ${YAC_SRC}

$(BUILD)/%.o: $(SRC_DIR)/%.c
	$(info ---------- Compile $< ----------)
	gcc ${INCLUDES} ${DEBUG} -g -c $< -o $@

main.c:
	$(info ---------- Compile ${MAIN_SRC} ----------)
	gcc ${INCLUDES} ${DEBUG} -g -c ${MAIN_SRC} -o ${MAIN_OUT}

${COMPILER}: create_build_folder lex.yy.c y.tab.c main.c ${OBJ}
	$(info ---------- Create compiler ----------)
	gcc ${CFLAGS} ${DEBUG} ${INCLUDES} -o ${COMPILER_OUT} -iquote ./ -iquote ../ ${LEX_OUT} ${YAC_OUT} ${MAIN_OUT} ${OBJ}

run:
	@./${COMPILER_OUT}

judge:
	@/home/share/hw2/judge.sh

clean:
	rm -rf ${BUILD}
