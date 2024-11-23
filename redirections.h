#ifndef REDIRECTIONS_H
#define REDIRECTIONS_H

#include "utilities.h"

typedef struct redir
{
	e_token_type	redir_type;
	t_string		filename;
	struct redir	*next;
}	t_redirection;

#endif
