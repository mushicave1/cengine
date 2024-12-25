build-run: compile
	build/main.o

compile: src/main.c
	cc -o build/main.o src/main.c 

clean: build
	rm -f build/**

.phony: build-run