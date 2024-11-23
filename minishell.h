#ifndef MINISHELL_H
#define MINISHELL_H

#include "utilities.h"
#include "input_handler.h"
#include "tokenizer.h"
#include "parser.h"

typedef struct shell_data
{
	t_string	u_promt;
	t_token		*tokens;
	t_ast_node	*ast_tree;
}	t_shell_data;

#endif
