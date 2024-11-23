#include "parser.h"

int	is_op(e_token_type token_type)
{
	return (token_type == PIPE);
}

int	is_redir(e_token_type token_type)
{
	return (token_type >= 1 && token_type <= 4);
}

int	get_op_priority(e_token_type type)
{
	if (type == PIPE)
		return (0);
	if (is_redir(type))
		return (1);
	return (-1);
}

void	add_redir(t_ast_node *cur_cmd_node, t_string *filename, e_token_type type)
{
	t_redirection	*n_redir;

	n_redir = (t_redirection *)malloc(sizeof(t_redirection));
	n_redir->filename = create_string(filename->data, filename->size);
	n_redir->redir_type = type;
	n_redir->next = cur_cmd_node->t_command.redirs;
	cur_cmd_node->t_command.redirs = n_redir;
}
