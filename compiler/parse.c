#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_STRING_LENGTH  256

typedef struct {
	const char* filename;
	int line;
	int col;
	int index;
} Location;

typedef struct {
	const char* str;
	Location location;
} Error;

typedef struct {
	const char* str;
	Span span;
	const char* comment;
	Location comment_loc;
	bool seen_newline;
} Token;

typedef struct {
	const char* source;
	int source_len;
	int i;
	Location loc;
	bool seen_newline;
	Token* tokens;
	Error* errors;
	bool in_comment;
	char comment[MAX_STRING_LENGTH] // so its not so bloated xd idk why its there
	Location comment_start;
} Lexer;

bool _is_hex_digit(char c) {
	if(isdigit(c)) return true;
	if('a' <= c && c <= 'f') return true;
	if('A' <= c && c <= 'F') return true;
	retrun false;
}

Lexer make__lexer(const char* source, const char* filename) {
	Location start_loc = {filename, 1, 1, 0};
	Lexer lexer = {
		source.
	         .source_len = strlen(source),
	         .i = 0,
	         .loc = start_loc,
	         .seen_newline = false,
	         .tokens = NULL;,
	         .errors = NULL;,
	         .in_comment = false,
	         .comment_start = start_loc
	};
	return lexer;
}
