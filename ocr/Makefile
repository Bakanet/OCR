CC=gcc
 
CPPFLAGS= `pkg-config --cflags gtk+-3.0` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3
LDFLAGS= $(PTHREAD) $(GTKLIB) -export-dynamic -rdynamic
LDLIBS= -lm `pkg-config --libs sdl2` `pkg-config --libs gtk+-3.0` \
        -lSDL2_image -rdynamic

SRC:=$(shell find . -name '*.c')
OBJ=$(SRC:.c=.o)
DEP=${OBJ:.o=.d}

all: main
 
main: ${OBJ}
 
clean:
	${RM} ${OBJ} ${DEP} *~
	${RM} main
 
-include ${DEPwildcard *.c}
