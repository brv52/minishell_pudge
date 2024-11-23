#include "parser.h"

void	st_init(t_stack *st)
{
	st->top = NULL;
}

void	destroy_node(t_node_stack *node)
{
	free_memo((void *)node);
}

void	destroy_stack(t_stack *st)
{
	t_node_stack	*current;
	t_node_stack	*next;

	current = st->top;
	while (current)
	{
		next = current->next;
		if (current->data)
			destroy_ast_tree((t_ast_node *)current->data);
		destroy_node(current);
		current = next;
	}
	st->top = NULL;
}
