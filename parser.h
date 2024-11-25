#ifndef PARSER_H
#define PARSER_H

#include "utilities.h"
#include "tokenizer.h"
#include "redirections.h"

typedef enum ast_node_type
{
	COMMAND, OPERATOR
}	e_ast_node_type;

typedef enum op_priority
{
	OP_PIPE, OP_REDIR_IN, OP_REDIR_OUT, OP_APPEND, OP_HEREDOC
}	e_op_priority;

typedef struct ast_node
{
	e_ast_node_type node_type;
	union
	{
		struct
		{
			char			**argv;
			size_t			argc;
		}	t_command;
		struct
		{
			e_token_type	op_type;
			struct ast_node	*left;
			struct ast_node	*right;
		}	t_operator;
	};
}	t_ast_node;

typedef struct node_stack
{
	void				*data;
	int					type;
	struct node_stack	*next;
}	t_node_stack;

typedef struct stack
{
	t_node_stack	*top;
}	t_stack;

typedef struct parser_state
{
	t_token			*current;
	t_ast_node		*cur_cmd_node;
	t_stack			op_stack;
	t_stack			out_stack;
	int				ret_state;
}	t_parser_state;

void		st_init(t_stack *st);
void		destroy_node(t_node_stack *node);
void		destroy_stack(t_stack *st);
void		st_push(t_stack *st, void *data, int type);
void		*st_pop(t_stack *st);
void		*peek(t_stack *st);
int			is_empty(t_stack *st);
int			is_op(e_token_type token_type);
int			is_redir(e_token_type token_type);
int			get_op_priority(e_token_type type);
int			pop_crt_node(t_stack *op_stack, t_stack *out_stack);
t_ast_node	*parse_tokens(t_token *tokens);
void		destroy_ast_tree(t_ast_node *node);

#endif
