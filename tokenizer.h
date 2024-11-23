#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "utilities.h"

#define __OP_LAST_IDX 4

typedef enum token_type
{
	PIPE, REDIR_IN, REDIR_OUT, HEREDOC, APPEND, WORD, S_QUOTE, D_QUOTE, L_BRACKET, R_BRACKET, UNDEFINED
}	e_token_type;

typedef struct token
{
	t_string		data;
	e_token_type	type;
	struct token	*next;
}	t_token;

t_token	*tokenize(t_string *u_promt);
void	destroy_token(t_token *token);
void	destroy_tokens(t_token *tokens);

#endif
