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


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_STRING_LENGTH 256

typedef struct {
    const char* filename;
    int line;
    int col;
    int index;
} Location;

typedef struct {
    const char* str;
    int len;
} Span;

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
    char comment[MAX_STRING_LENGTH];
    Location comment_start;
} Lexer;

bool is_hex_digit(char c) {
    if (isdigit(c)) return true;
    if ('a' <= c && c <= 'f') return true;
    if ('A' <= c && c <= 'F') return true;
    return false;
}

Lexer make_lexer(const char* source, const char* filename) {
    Location start_loc = {filename, 1, 1, 0};
    Lexer lexer = {
        source,
        .source_len = strlen(source),
        .i = 0,
        .loc = start_loc,
        .seen_newline = false,
        .tokens = NULL,
        .errors = NULL,
        .in_comment = false,
        .comment_start = start_loc
    };
    return lexer;
}

void push(Token* token, Lexer* lexer) {
    token->seen_newline = lexer->seen_newline;
    if (strlen(lexer->comment) > 0) {
        token->comment = strdup(lexer->comment);
        token->comment_loc = lexer->comment_start;
    }
    lexer->comment[0] = '\0';

    lexer->tokens = realloc(lexer->tokens, (sizeof(Token) * (lexer->num_tokens + 1)));
    lexer->tokens[lexer->num_tokens++] = *token;

    lexer->seen_newline = false;
    lexer->in_comment = false;
}

void push_type(TokenType type, int len, Lexer* lexer) {
    Location start_loc = lexer->loc;
    for (int i = 0; i < len; ++i) {
        inc(lexer);
    }

    Token token = make_token(type, make_span(start_loc, lexer->loc));
    push(&token, lexer);
}

char cur(const Lexer* lexer) {
    return lexer->source[lexer->i];
}

void inc(Lexer* lexer) {
    switch (cur(lexer)) {
        case '\n':
            lexer->loc.line += 1;
            lexer->loc.col = 1;
            lexer->seen_newline = true;
            break;
        default:
            lexer->loc.col += 1;
    }
    lexer->i += 1;
    lexer->loc.index += 1;
}

char peek(const Lexer* lexer, int offset) {
    if (cur(lexer) == '\0') {
        return cur(lexer);
    }
    return lexer->source[lexer->i + offset];
}

void lex_char_literal(Lexer* lexer) {
    Location start_loc = lexer->loc;
    int start = lexer->i + 1;
    inc(lexer);

    if (cur(lexer) == '\\') {
        inc(lexer);
    }
    inc(lexer);
    if (cur(lexer) != '\'') {
        Error error = make_error(make_span(start_loc, lexer->loc), "Expected ' after character literal");
        lexer->errors = realloc(lexer->errors, (sizeof(Error) * (lexer->num_errors + 1)));
        lexer->errors[lexer->num_errors++] = error;
    }

    int len = lexer->i - start;
    const char* text = substring(lexer->source, start, len);

    inc(lexer);
    Token token = make_token(TokenType_CharLiteral, make_span(start_loc, lexer->loc), text);
    push(&token, lexer);
}
