all: nnpkm

CFLAGS=-I../include

H_DEPS=$(wildcard ../include/nnl/*.h)

obj/shared/%.o: ../shared/%.c $(H_DEPS)
	mkdir -p $(@D)
	gcc -c -o $@ $< $(CFLAGS)

obj/%.o: ./%.c $(H_DEPS)
	gcc -c -o $@ $< $(CFLAGS)

NNPKM_FILES=$(wildcard ./*.c)
SHARED_FILES=$(wildcard ../shared/*.c)
OBJ_FILES=$(SHARED_FILES:../shared/%.c=obj/shared/%.o)
OBJ_FILES+=$(NNPKM_FILES:./%.c=obj/%.o)

nnpkm: $(OBJ_FILES)
	gcc -o ./nnpkm $(OBJ_FILES) -lyaml -lcurl

clean:
	rm -f nnpkm
	rm -rf obj