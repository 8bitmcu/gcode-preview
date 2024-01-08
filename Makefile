
SRC = main.c Turbo-Base64/turbob64d.c
OBJ = ${SRC:.c=.o}

all: options gcode-preview

options:
	@echo gcode-preview build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

.c.o:
	${CC} -c ${CFLAGS} $<

gcode-preview: ${OBJ}
	${CC} -o $@ main.o turbob64d.o ${LDFLAGS}

clean:
	rm -f gcode-preview main.o turbob64d.o

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f gcode-preview ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/gcode-preview

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/gcode-preview

.PHONY: all options clean install uninstall
