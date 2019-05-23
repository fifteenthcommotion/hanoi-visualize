/* required for nanosleep */
#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h> /* atoi */
#include <string.h> /* memset */
#include <time.h> /* nanosleep */

enum {
	LEFT,
	MIDDLE,
	RIGHT
};

struct timespec point_two_seconds = {
  .tv_sec = 0,
  .tv_nsec = 200000000
};

unsigned int max(unsigned int a, unsigned int b) {
	if (a > b) {
		return a;
	} else {
		return b;
	}
}

/*
board[0 <= x <= n-1] : layer at height x
board[n] : heights of stacks
*/

void init_board(unsigned int board[][3], unsigned int n) {
	unsigned int i;
	memset(board, 0, sizeof(int) * (n+1) * 3);
	for (i = 0; i < n; ++i) {
		board[i][LEFT] = n - i;
	}
	board[n][LEFT] = n;
}

void print_board(unsigned int board[][3], unsigned int n) {
	unsigned int piece;
	unsigned int i;
	unsigned char stack;
	char str[n * 16 + 1];
	size_t offset = 0;
	nanosleep(&point_two_seconds, NULL);
	for (i = 0; i < n; ++i) {
		for (stack = 0; stack < 3; ++stack) {
			piece = board[n - i - 1][stack];
			if (piece == 0) {
				sprintf(str + offset, "     ");
				offset += 5;
			} else {
				sprintf(str + offset, "%4d ", piece);
				offset += 5;
			}
		}
		sprintf(str + offset, "\n");
		offset += 1;
	}
	str[n * 16] = 0;
	printf("\033[2J\033[H%s", str); /* clear and print new output */
	fflush(stdout);
}

void move(unsigned int board[][3], unsigned int board_n, unsigned int n, unsigned char from, unsigned char to) {
	unsigned char other;
	unsigned int from_height;
	unsigned int to_height;
	unsigned int i;

	/* n must never be zero */
	if (n == 1) {
		from_height = board[board_n][from];
		to_height = board[board_n][to];
		for (i = 0; i < n; ++i) {
			board[to_height + i][to] = board[from_height - n][from];
			board[from_height - n][from] = 0;
		}
		board[board_n][to] += n;
		board[board_n][from] -= n;
		print_board(board, board_n);
	} else {
		/* n > 1 */
		other = (3 - from - to) % 3;
		move(board, board_n, n-1, from, other);
		move(board, board_n, 1, from, to);
		move(board, board_n, n-1, other, to);
	}
}

int main(int argc, char **argv) {
	int n;

	if (argc != 2) {
		fprintf(stderr, "usage: ./hanoi-visualize <n>\n");
		exit(EXIT_FAILURE);
	}

	n = atoi(argv[1]);
	if (n <= 0 || n >= 10000) {
		fprintf(stderr, "n must be 0 < n < 10000, got n=%d\n", n);
		exit(EXIT_FAILURE);
	}

	unsigned int board[n+1][3];
	init_board(board, n);

	print_board(board, n);
	move(board, n, n, LEFT, RIGHT);
	printf("\n");

	return 0;
}
