#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

static const char *input;
static int pos;

void lexer_init(const char *source)
{
    input = source;
    pos = 0;
}

static void skip_whitespace()
{
    while (isspace(input[pos]))
        pos++;
}

static int starts_with(const char *str)
{
    return strncmp(&input[pos], str, strlen(str)) == 0;
}

static int safe_peek(int offset)
{
    if (input[pos + offset] == '\0')
        return 0;
    return input[pos + offset];
}

Token lexer_next_token()
{
    Token token;
    token.type = TOK_UNKNOWN;
    token.text[0] = '\0';

    skip_whitespace();

    if (input[pos] == '\0')
    {
        token.type = TOK_EOF;
        return token;
    }

    // Handle comments starting with '#'
    if (input[pos] == '#')
    {
        while (input[pos] != '\n' && input[pos] != '\0')
            pos++;
        return lexer_next_token(); // Skip the comment and get the next token
    }

    // Keywords and multi-char tokens (order matters)
    if (starts_with("let$"))
    {
        token.type = TOK_LET;
        strcpy(token.text, "let$");
        pos += 4;
        return token;
    }
    if (starts_with("::print"))
    {
        token.type = TOK_PRINT;
        strcpy(token.text, "::print");
        pos += 7;
        return token;
    }
    if (starts_with("if$"))
    {
        token.type = TOK_IF;
        strcpy(token.text, "if$");
        pos += 3;
        return token;
    }
    if (starts_with("elseif$"))
    {
        token.type = TOK_ELSEIF;
        strcpy(token.text, "elseif$");
        pos += 7;
        return token;
    }
    if (starts_with("else"))
    {
        token.type = TOK_ELSE;
        strcpy(token.text, "else");
        pos += 4;
        return token;
    }
    if (starts_with("loop$"))
    {
        token.type = TOK_LOOP;
        strcpy(token.text, "loop$");
        pos += 5;
        return token;
    }
    if (starts_with("import$"))
    {
        token.type = TOK_IMPORT;
        strcpy(token.text, "import$");
        pos += 7;
        return token;
    }
    if (starts_with("func$"))
    {
        token.type = TOK_FUNC;
        strcpy(token.text, "func$");
        pos += 5;
        return token;
    }
    if (starts_with("class$"))
    {
        pos += 6;
        token.type = TOK_CLASS;
        strcpy(token.text, "class$");
        return token;
    }
    if (starts_with("self"))
    {
        pos += 4;
        token.type = TOK_SELF;
        strcpy(token.text, "self");
        return token;
    }

    if (starts_with("and"))
    {
        token.type = TOK_AND;
        strcpy(token.text, "and");
        pos += 3;
        return token;
    }
    if (starts_with("or"))
    {
        token.type = TOK_OR;
        strcpy(token.text, "or");
        pos += 2;
        return token;
    }
    if (starts_with("not"))
    {
        token.type = TOK_NOT;
        strcpy(token.text, "not");
        pos += 3;
        return token;
    }

    // Assignment operator :=
    if (input[pos] == ':' && safe_peek(1) == '=')
    {
        token.type = TOK_ASSIGN;
        strcpy(token.text, ":=");
        pos += 2;
        return token;
    }

    // Arrow operator =>
    if (input[pos] == '=' && safe_peek(1) == '>')
    {
        token.type = TOK_ARROW;
        strcpy(token.text, "=>");
        pos += 2;
        return token;
    }

    // Arrow operator ⟶ (UTF-8: E2 87 92)
    if ((unsigned char)input[pos] == 0xE2 && (unsigned char)safe_peek(1) == 0x87 && (unsigned char)safe_peek(2) == 0x92)
    {
        token.type = TOK_ARROW;
        strcpy(token.text, "⟶");
        pos += 3;
        return token;
    }

    if (starts_with("::len"))
    {
        token.type = TOK_LIST_LEN;
        strcpy(token.text, "::len");
        pos += 5;
        return token;
    }
    if (starts_with("::append"))
    {
        token.type = TOK_LIST_APPEND;
        strcpy(token.text, "::append");
        pos += 8;
        return token;
    }
    if (starts_with("::prepend"))
    {
        token.type = TOK_LIST_PREPEND;
        strcpy(token.text, "::prepend");
        pos += 9;
        return token;
    }
    if (starts_with("::pop"))
    {
        token.type = TOK_LIST_POP;
        strcpy(token.text, "::pop");
        pos += 5;
        return token;
    }
    if (starts_with("::insert"))
    {
        token.type = TOK_LIST_INSERT;
        strcpy(token.text, "::insert");
        pos += 8;
        return token;
    }
    if (starts_with("::remove"))
    {
        token.type = TOK_LIST_REMOVE;
        strcpy(token.text, "::remove");
        pos += 8;
        return token;
    }
    if (starts_with("::pattern_match"))
    {
        token.type = TOK_PATTERN_MATCH;
        strcpy(token.text, "::pattern_match");
        pos += 15;
        return token;
    }

    // Single char tokens and operators
    switch (input[pos])
    {
    case '+':
        token.type = TOK_PLUS;
        token.text[0] = '+';
        token.text[1] = '\0';
        pos++;
        return token;
    case '-':
        token.type = TOK_MINUS;
        token.text[0] = '-';
        token.text[1] = '\0';
        pos++;
        return token;
    case '*':
        token.type = TOK_MUL;
        token.text[0] = '*';
        token.text[1] = '\0';
        pos++;
        return token;
    case '/':
        token.type = TOK_DIV;
        token.text[0] = '/';
        token.text[1] = '\0';
        pos++;
        return token;
    case '%':
        token.type = TOK_MOD;
        token.text[0] = '%';
        token.text[1] = '\0';
        pos++;
        return token;
    case '(':
        token.type = TOK_LPAREN;
        token.text[0] = '(';
        token.text[1] = '\0';
        pos++;
        return token;
    case ')':
        token.type = TOK_RPAREN;
        token.text[0] = ')';
        token.text[1] = '\0';
        pos++;
        return token;
    case ';':
        token.type = TOK_SEMICOLON;
        token.text[0] = ';';
        token.text[1] = '\0';
        pos++;
        return token;
    case '>':
        if (safe_peek(1) == '=')
        {
            token.type = TOK_GTE;
            strcpy(token.text, ">=");
            pos += 2;
        }
        else
        {
            token.type = TOK_GT;
            token.text[0] = '>';
            token.text[1] = '\0';
            pos++;
        }
        return token;
    case '<':
        if (safe_peek(1) == '=')
        {
            token.type = TOK_LTE;
            strcpy(token.text, "<=");
            pos += 2;
        }
        else
        {
            token.type = TOK_LT;
            token.text[0] = '<';
            token.text[1] = '\0';
            pos++;
        }
        return token;
    case '=':
        if (safe_peek(1) == '=')
        {
            token.type = TOK_EQ;
            strcpy(token.text, "==");
            pos += 2;
            return token;
        }
        break;
    case '!':
        if (safe_peek(1) == '=')
        {
            token.type = TOK_NEQ;
            strcpy(token.text, "!=");
            pos += 2;
            return token;
        }
        break;
    case ',':
        token.type = TOK_COMMA;
        token.text[0] = ',';
        token.text[1] = '\0';
        pos++;
        return token;
    case '{':
        token.type = TOK_LBRACE;
        token.text[0] = '{';
        token.text[1] = '\0';
        pos++;
        return token;
    case '}':
        token.type = TOK_RBRACE;
        token.text[0] = '}';
        token.text[1] = '\0';
        pos++;
        return token;
    case '[':
        token.type = TOK_LBRACKET;
        token.text[0] = '[';
        token.text[1] = '\0';
        pos++;
        return token;
    case ']':
        token.type = TOK_RBRACKET;
        token.text[0] = ']';
        token.text[1] = '\0';
        pos++;
        return token;
    case '&':
        token.type = TOK_BITWISE_AND;
        token.text[0] = '&';
        token.text[1] = '\0';
        pos++;
        return token;
    case '|':
        token.type = TOK_BITWISE_OR;
        token.text[0] = '|';
        token.text[1] = '\0';
        pos++;
        return token;
    case '^':
        token.type = TOK_BITWISE_XOR;
        token.text[0] = '^';
        token.text[1] = '\0';
        pos++;
        return token;
    case '~':
        token.type = TOK_BITWISE_NOT;
        token.text[0] = '~';
        token.text[1] = '\0';
        pos++;
        return token;
    case '@':
        token.type = TOK_FORMAT_SPECIFIER;
        token.text[0] = '@';
        token.text[1] = '\0';
        pos++;
        return token;
    case '.':
        token.type = TOK_DOT;
        token.text[0] = '.';
        token.text[1] = '\0';
        pos++;
        return token;
    }
    // String literal
    if (input[pos] == '"')
    {
        pos++;
        int start = pos;
        while (input[pos] != '"' && input[pos] != '\0')
        {
            // Handle escaped quotes
            if (input[pos] == '\\' && input[pos + 1] == '"')
            {
                pos += 2;
            }
            else
            {
                pos++;
            }
        }
        int len = pos - start;
        if (len >= sizeof(token.text))
            len = sizeof(token.text) - 1;
        strncpy(token.text, &input[start], len);
        token.text[len] = '\0';
        token.type = TOK_STRING;
        if (input[pos] == '"')
            pos++;
        return token;
    }

    // Number literal (integer or float)
    if (isdigit(input[pos]))
    {
        int start = pos;
        while (isdigit(input[pos]) || input[pos] == '.')
            pos++;
        int len = pos - start;
        if (len >= sizeof(token.text))
            len = sizeof(token.text) - 1;
        strncpy(token.text, &input[start], len);
        token.text[len] = '\0';
        token.type = TOK_NUMBER;
        return token;
    }

    // Identifier
    if (isalpha(input[pos]) || input[pos] == '@' || input[pos] == '$' || input[pos] == '_')
    {
        int start = pos;
        while (isalnum(input[pos]) || input[pos] == '@' || input[pos] == '$' || input[pos] == '_')
            pos++;
        int len = pos - start;
        if (len >= sizeof(token.text))
            len = sizeof(token.text) - 1;
        strncpy(token.text, &input[start], len);
        token.text[len] = '\0';
        token.type = TOK_ID;
        return token;
    }

    token.type = TOK_UNKNOWN;
    token.text[0] = input[pos];
    token.text[1] = '\0';
    pos++;
    return token;
}