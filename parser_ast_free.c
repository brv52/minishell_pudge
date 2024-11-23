#include "parser.h"

static void	free_argv(t_ast_node *node)
{
	size_t	c_arg;

	c_arg = 0;
	while (c_arg < node->t_command.argc)
	{
		destroy_str(&node->t_command.argv[c_arg]);
		c_arg += 1;
	}
	free_memo((void *)node->t_command.argv);
	node->t_command.argv = NULL;
	node->t_command.argc = 0;
}

static void	free_redirs(t_ast_node *node)
{
	t_redirection	*c_redir;
	t_redirection	*n_redir;

	c_redir = node->t_command.redirs;
	while (c_redir)
	{
		n_redir = c_redir->next;
		destroy_str(&c_redir->filename);
		free_memo((void *)c_redir);
		c_redir = n_redir;
	}
}

void	destroy_ast_tree(t_ast_node *node)
{
	if (node == NULL) return;

	if (node->node_type == COMMAND)
	{
		free_argv(node);
		free_redirs(node);
	}
	else if (node->node_type == OPERATOR)
	{
		destroy_ast_tree(node->t_operator.left);
		destroy_ast_tree(node->t_operator.right);
	}
	free_memo((void *)node);
}
