.PHONY: all

all: hanoi-visualize

hanoi-visualize: hanoi-visualize.c
	gcc -O3 -std=c89 -Wall -o hanoi-visualize hanoi-visualize.c
