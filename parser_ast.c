#include "parser.h"

static t_ast_node	*crt_command(t_token *current)
{
	t_ast_node	*cur_cmd_node;

	cur_cmd_node = (t_ast_node *)malloc(sizeof(t_ast_node));
	cur_cmd_node->node_type = COMMAND;
	cur_cmd_node->t_command.argv = (t_string *)malloc(sizeof(t_string) * 1);
	cur_cmd_node->t_command.argv[0] = create_string(current->data.data, current->data.size);
	cur_cmd_node->t_command.argc = 1;
	cur_cmd_node->t_command.redirs = NULL;
	return (cur_cmd_node);
}

static void	add_arg(t_ast_node *cur_cmd_node, t_token *current)
{
	size_t		c_arg;
	t_string	*n_argv;

	c_arg = 0;
	n_argv = (t_string *)malloc(sizeof(t_string) * (cur_cmd_node->t_command.argc + 1));
	while (c_arg < cur_cmd_node->t_command.argc)
	{
		n_argv[c_arg] = create_string(cur_cmd_node->t_command.argv[c_arg].data, cur_cmd_node->t_command.argv[c_arg].size);
		destroy_str(&cur_cmd_node->t_command.argv[c_arg]);
		c_arg += 1;
	}
	n_argv[c_arg] = create_string(current->data.data, current->data.size);
	free_memo((void *)cur_cmd_node->t_command.argv);
	cur_cmd_node->t_command.argv = n_argv;
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
	if(state->cur_cmd_node)
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
	if (state->cur_cmd_node == NULL)
	{
		state->cur_cmd_node = crt_command(state->current);
		st_push(&state->out_stack, state->cur_cmd_node);
	}
	else
		add_arg(state->cur_cmd_node, state->current);
}

void	handle_redir(t_parser_state *state)
{
	e_token_type tmp_type;

	if (!state->cur_cmd_node)
	{
		state->ret_state = cleanup_parser(state, 0);
		return ;
	}
	if (!state->current)
	{
		state->ret_state = cleanup_parser(state, 1);
		return ;
	}
	tmp_type = state->current->type;
	state->current = state->current->next;
	if (state->current->type != WORD)
	{
		state->ret_state = cleanup_parser(state, 1);
		return ;
	}
	add_redir(state->cur_cmd_node, &state->current->data, tmp_type);
}

void	handle_l_bracket(t_parser_state *state)
{
	state->cur_cmd_node = NULL;
	st_push(&state->op_stack, state->current);
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
	st_push(&state->op_stack, state->current);
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
		else if (is_redir(state.current->type))
			handle_redir(&state);
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
