#include "parser.h"

static t_ast_node	*crt_op_node(e_token_type type, t_ast_node *left, t_ast_node *right)
{
	t_ast_node	*op_node;

	op_node = (t_ast_node *)malloc(sizeof(t_ast_node));
	op_node->node_type = OPERATOR;
	op_node->t_operator.op_type = type;
	op_node->t_operator.left = left;
	op_node->t_operator.right = right;
	return (op_node);
}

static void	decide_free(t_ast_node *left, t_ast_node *right)
{
	if (left)
		destroy_ast_tree(left);
	if (right)
		destroy_ast_tree(right);
}

int	pop_crt_node(t_stack *op_stack, t_stack *out_stack)
{
	t_token		*op_token;
	t_ast_node	*left;
	t_ast_node	*right;

	op_token = (t_token *)st_pop(op_stack);
	if (!op_token)
	{
		printf("Operator stack is empty\n");
		return (0);
	}
	if (op_token->type == L_BRACKET)
	{
		printf("Missmatched paratheses\n");
		return (0);
	}
	left = NULL;
	right = (t_ast_node *)st_pop(out_stack);
	if (is_op(op_token->type))
	{
		left = (t_ast_node *)st_pop(out_stack);
		if (!left || !right)
		{
			decide_free(left, right);
			printf("Not enough operands\n");
			return (0);
		}
	}
	st_push(out_stack, crt_op_node(op_token->type, left, right), 1);
	return (1);
}
