CFLAGS := -Wall -O0 -ggdb
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

COMPILER_OUT := ${BUILD}/${BUILD_OUT}/${COMPILER}
LEX_OUT := ${BUILD}/lex.yy.c
YAC_OUT := ${BUILD}/y.tab.c
MAIN_OUT := ${BUILD}/main.o
JAVA_ASM_OUT := ${BUILD}/Main.j

all: build run

build: ${COMPILER}

.PHONY: main.c

create_build_folder:
	mkdir -p ${BUILD}
	mkdir -p ${BUILD}/${BUILD_OUT}

lex.yy.c:
	$(info ---------- Compile Lex ----------)
	lex -o ${LEX_OUT} ${LEX_SRC}

y.tab.c:
	$(info ---------- Compile Yacc ----------)
	yacc ${YFLAG} -o ${YAC_OUT} ${YAC_SRC}

main.c:
	$(info ---------- Compile ${MAIN_SRC} ----------)
	gcc -g -c ${MAIN_SRC} -o ${MAIN_OUT}

${COMPILER}: create_build_folder lex.yy.c y.tab.c main.c
	$(info ---------- Create compiler ----------)
	gcc ${CFLAGS} -o ${COMPILER_OUT} -iquote ./ -iquote ../ ${LEX_OUT} ${YAC_OUT} ${MAIN_OUT}

run:
	@./${COMPILER_OUT}

judge:
	@/home/share/hw2/judge.sh

clean:
	rm -rf ${BUILD}
