#include "parser.h"

int	is_op(e_token_type token_type)
{
	return (token_type >= 0 && token_type <= 4);
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
