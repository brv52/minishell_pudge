#include "parser.h"

void	st_push(t_stack *st, void *data, int type)
{
	t_node_stack	*node;

	node = (t_node_stack *)malloc(sizeof(t_node_stack));
	node->data = data;
	node->type = type;
	node->next = st->top;
	st->top = node;
}
void	*st_pop(t_stack *st)
{
	t_node_stack	*node;
	void			*data_snap;

	if (!st->top)
		return (NULL);
	node = st->top;
	data_snap = node->data;
	st->top = node->next;
	free_memo((void **)&node);
	return (data_snap);
}
void	*peek(t_stack *st)
{
	if (!st->top)
		return (NULL);
	return (st->top->data);
}

int	is_empty(t_stack *st)
{
	return (st->top == NULL);
}
