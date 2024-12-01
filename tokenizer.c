#include "tokenizer.h"

static e_token_type	get_token_type(t_string *str)
{
	if (cmp_str_data(str->data, "|") == 0)
		return (PIPE);
	else if (cmp_str_data(str->data, "<") == 0)
		return (REDIR_IN);
	else if (cmp_str_data(str->data, ">") == 0)
		return (REDIR_OUT);
	else if (cmp_str_data(str->data, "<<") == 0)
		return (HEREDOC);
	else if (cmp_str_data(str->data, ">>") == 0)
		return (APPEND);
	else if (str->data && str->data[0] == '(')
		return (L_BRACKET);
	else if (str->data && str->data[0] == ')')
		return (R_BRACKET);
	return (WORD);
}

void	destroy_token(t_token *token)
{
	destroy_str(&token->data);
	free_memo((void **)&token);
}

void	destroy_tokens(t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	if (!tokens || tokens == NULL)
		return ;
	current = tokens;
	while (current)
	{
		next = current->next;
		destroy_token(current);
		current = next;
	}
}

static void	handle_quotes(t_string *u_promt, size_t *c_pos, t_token *new)
{
	char	quote;
	size_t	start;

	quote = u_promt->data[*c_pos];
	*c_pos += 1;
	start = *c_pos;
	while (*c_pos < u_promt->size && u_promt->data[*c_pos] != quote)
		*c_pos += 1;
	new->data = create_string(&u_promt->data[start], *c_pos - start);
	*c_pos += 1;
	if (quote == '\'')
		new->type = S_QUOTE;
	else if (quote == '"')
		new->type = D_QUOTE;
}

static void	handle_metacharacters(t_string *u_promt, size_t *c_pos, t_token *new)
{
	if ((u_promt->data[*c_pos] == '&' || u_promt->data[*c_pos] == '|' || u_promt->data[*c_pos] == '<' || u_promt->data[*c_pos] == '>') && u_promt->data[*c_pos + 1] == u_promt->data[*c_pos])
	{
		new->data = create_string(&u_promt->data[*c_pos], 2);
		*c_pos += 2;
	}
	else
	{
		new->data = create_string(&u_promt->data[*c_pos], 1);
		*c_pos += 1;
	}
}

static void	handle_words(t_string *u_promt, size_t *c_pos, t_token *new)
{
	size_t	start;

	start = *c_pos;
	while (*c_pos < u_promt->size && u_promt->data[*c_pos] != ' ' && str_chr(u_promt->data[*c_pos], __METACHARS) == -1)
		*c_pos += 1;
	new->data = create_string(&u_promt->data[start], *c_pos - start);
}

static void	handle_token(t_string *u_promt, size_t *c_pos, t_token *new)
{
	new->type = UNDEFINED;
	if (u_promt->data[*c_pos] == '\'' || u_promt->data[*c_pos] == '"')
		handle_quotes(u_promt, c_pos, new);
	else if (str_chr(u_promt->data[*c_pos], __METACHARS) != -1)
		handle_metacharacters(u_promt, c_pos, new);
	else
		handle_words(u_promt, c_pos, new);
	if (new->type == UNDEFINED)
		new->type = get_token_type(&new->data);
}

t_token	*tokenize(t_string *u_promt)
{
	t_token	*head;
	t_token	**current;
	t_token	*new;
	size_t	c_pos;

	head = NULL;
	current = &head;
	c_pos = 0;
	while (c_pos < u_promt->size)
	{
		if (u_promt->data[c_pos] == ' ')
		{
			c_pos += 1;
			continue ;
		}
		new = (t_token *)malloc(sizeof(t_token));
		new->next = NULL;
		handle_token(u_promt, &c_pos, new);
		*current = new;
		current = &new->next;
	}
	return (head);
}
