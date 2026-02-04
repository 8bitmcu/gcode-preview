PREFIX = /usr/local

# Includes and libs
CFLAGS = -std=gnu99 -pedantic -Wall -O3
LDFLAGS = -s

SRC = gcode-preview.c Turbo-Base64/turbob64d.c
OBJ = ${SRC:.c=.o}
CROSS = x86_64-w64-mingw32-
W_CC = ${CROSS}gcc

all: options gcode-preview

options:
	@echo gcode-preview build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

# Pattern rule for better subdirectory handling
%.o: %.c
	${CC} -c ${CFLAGS} $< -o $@

# Explicitly link all objects
gcode-preview: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

windows:
	${W_CC} ${CFLAGS} -o gcode-preview.exe ${SRC} ${LDFLAGS} -static

clean:
	rm -f gcode-preview ${OBJ}

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f gcode-preview ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/gcode-preview

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/gcode-preview

.PHONY: all options clean install uninstall
