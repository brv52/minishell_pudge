#include "parser.h"

static t_ast_node	*crt_command(t_token *current)
{
	t_ast_node	*cur_cmd_node;

	cur_cmd_node = (t_ast_node *)malloc(sizeof(t_ast_node));
	cur_cmd_node->node_type = COMMAND;
	cur_cmd_node->t_command.argv = (char **)malloc(sizeof(char *) * 2);
	cur_cmd_node->t_command.types = (e_token_type *)malloc(sizeof(e_token_type) * 2);
	cur_cmd_node->t_command.argv[0] = cp_str_data(current->data.data);
	cur_cmd_node->t_command.types[0] = current->type;
	cur_cmd_node->t_command.types[1] = UNDEFINED;
	cur_cmd_node->t_command.argv[1] = NULL;
	cur_cmd_node->t_command.argc = 1;
	return (cur_cmd_node);
}

static void	add_arg(t_ast_node *cur_cmd_node, t_token *current)
{
	size_t			c_arg;
	char			**n_argv;
	e_token_type	*n_types;

	c_arg = 0;
	n_argv = (char **)malloc(sizeof(char *) * (cur_cmd_node->t_command.argc + 2));
	n_types = (e_token_type *)malloc(sizeof(e_token_type) * (cur_cmd_node->t_command.argc + 2));
	while (c_arg < cur_cmd_node->t_command.argc)
	{
		n_argv[c_arg] = cp_str_data(cur_cmd_node->t_command.argv[c_arg]);
		free_memo((void **)&cur_cmd_node->t_command.argv[c_arg]);
		n_types[c_arg] = cur_cmd_node->t_command.types[c_arg];
		c_arg += 1;
	}
	n_argv[c_arg] = cp_str_data(current->data.data);
	n_types[c_arg] = current->type;
	n_argv[c_arg + 1] = NULL;
	n_types[c_arg + 1] = UNDEFINED;
	free_memo((void **)&cur_cmd_node->t_command.argv);
	free_memo((void **)&cur_cmd_node->t_command.types);
	cur_cmd_node->t_command.argv = n_argv;
	cur_cmd_node->t_command.types = n_types;
	cur_cmd_node->t_command.argc += 1;
}

static int	cleanup_parser(t_parser_state *state, int err_code)
{
	if (err_code == 0)
		printf("Redirection without command\n");
	if (err_code == 1)
		printf("Filename expected\n");
	if (err_code == 2)
		printf("Mismatched parantheses\n");
	if (err_code == 3)
		printf("Unknown token type\n");
	if (err_code == 4)
		printf("Multiple nodes left on out stack\n");
	if (err_code == 5)
		printf("Empty out stack\n");
	destroy_stack(&state->op_stack);
	destroy_stack(&state->out_stack);
	// if (err_code != -1)
	if(err_code != -1 && err_code != 2 && err_code != 1 && state->cur_cmd_node)
	{
		destroy_ast_tree(state->cur_cmd_node);
		state->cur_cmd_node = NULL;
	}
	return (-1);
}

void initialize_parser(t_parser_state *state, t_token *tokens)
{
	state->current = tokens;
	state->cur_cmd_node = NULL;
	state->ret_state = 0;
	st_init(&state->op_stack);
	st_init(&state->out_stack);
}

void	handle_word(t_parser_state *state)
{
	printf("word handling: [%s]\n", state->current->data.data);
	if (state->cur_cmd_node == NULL)
	{
		state->cur_cmd_node = crt_command(state->current);
		st_push(&state->out_stack, state->cur_cmd_node, 1);
	}
	else
		add_arg(state->cur_cmd_node, state->current);
}

void	handle_l_bracket(t_parser_state *state)
{
	state->cur_cmd_node = NULL;
	st_push(&state->op_stack, state->current, 0);
}

void	handle_r_bracket(t_parser_state *state)
{
	// state->cur_cmd_node = NULL;
	while (!is_empty(&state->op_stack) && ((t_token *)peek(&state->op_stack))->type != L_BRACKET && get_op_priority(state->current->type) < get_op_priority(((t_token *)peek(&state->op_stack))->type))
	{
		if (!pop_crt_node(&state->op_stack, &state->out_stack))
		{
			state->ret_state = cleanup_parser(state, -1);
			return ;
		}
	}
	if (is_empty(&state->op_stack))
	{
		state->ret_state = cleanup_parser(state, 2);
		return ;
	}
	st_pop(&state->op_stack);
}

void	handle_op(t_parser_state *state)
{
	state->cur_cmd_node = NULL;
	while (!is_empty(&state->op_stack) && is_op(((t_token *)peek(&state->op_stack))->type) && get_op_priority(((t_token *)peek(&state->op_stack))->type) >= get_op_priority(state->current->type))
	{
		if (!pop_crt_node(&state->op_stack, &state->out_stack))
		{
			state->ret_state = (cleanup_parser(state, -1));
			return ;
		}
	}
	st_push(&state->op_stack, state->current, 0);
}

t_ast_node	*get_root(t_parser_state *state)
{
	t_ast_node *root;

	while (!is_empty(&state->op_stack))
	{
		if (!pop_crt_node(&state->op_stack, &state->out_stack))
		{
			state->ret_state = cleanup_parser(state, -1);
			return (NULL);
		}
	}
	if (!is_empty(&state->out_stack))
	{
		root = (t_ast_node *)st_pop(&state->out_stack);
		if (!is_empty(&state->out_stack))
		{
			state->ret_state = cleanup_parser(state, 4);
			return (NULL);
		}
		return (root);
	}
	else
	{
		state->ret_state = cleanup_parser(state, 5);
		return (NULL);
	}
}

t_ast_node	*parse_tokens(t_token *tokens)
{
	t_parser_state state;

	initialize_parser(&state, tokens);
	while (state.current)
	{
		printf("c_token: %s %d\n", state.current->data.data, state.current->type);
		if (state.current->type >= 5 && state.current->type <= 7)
			handle_word(&state);
		else if (state.current->type == L_BRACKET)
			handle_l_bracket(&state);
		else if (state.current->type == R_BRACKET)
			handle_r_bracket(&state);
		else if (is_op(state.current->type))
			handle_op(&state);
		else
			state.ret_state = cleanup_parser(&state, 3);
		if (state.ret_state == -1)
			return (NULL);
		state.current = state.current->next;
	}
	return (get_root(&state));
}
