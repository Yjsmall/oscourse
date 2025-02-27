all: hanoi-r hanoi-nr two-fun

hanoi-r: hanoi-main.c hanoi-r.c
	gcc -g -o $@ $^

hanoi-nr: hanoi-main.c hanoi-nr.c
	gcc -g -o $@ $^

two-fun: two-fun.c
	gcc -g -o@ $^

clean:
	rm -f hanoi-r hanoi-nr two-func
